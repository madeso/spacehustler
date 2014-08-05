// Euphoria - Copyright (c) Gustav

#include "euphoria/oculusvr.h"

#include <cassert>
#include <string>

#include "OVR.h"  // NOLINT this is how you should include OVR

EyeSetup::EyeSetup(float w, float h, float x, float y, const Mat44& projection,
                   const Mat44& view_adjust)
    : w_(w),
      h_(h),
      x_(x),
      y_(y),
      projection_(projection),
      view_adjust_(view_adjust) {}

float EyeSetup::w() const {
  assert(this);
  return w_;
}
float EyeSetup::h() const {
  assert(this);
  return h_;
}
float EyeSetup::x() const {
  assert(this);
  return x_;
}
float EyeSetup::y() const {
  assert(this);
  return y_;
}
const Mat44& EyeSetup::projection() const {
  assert(this);
  return projection_;
}
const Mat44& EyeSetup::view_adjust() const {
  assert(this);
  return view_adjust_;
}

Mat44 C(const OVR::Matrix4f& m) {
  // matrix4f is row-major
  // mat44 is col major
  /// @todo this seems wrong, but it works out for the better, perhaps the
  /// viewmatrix should be transposed too, so we need to transpose twice?
  return Mat44(m.M);
  // return cml::transpose(mat44(m.M));
}

ovrSizei SizeiMin(ovrSizei a, ovrSizei b) {
  ovrSizei ret;
  ret.w = (a.w  < b.w)  ? a.w  : b.w;
  ret.h = (a.h < b.h) ? a.h : b.h;
  return ret;
}

struct OculusSettings {
  bool VsyncEnabled;
  bool IsLowPersistence;
  bool DynamicPrediction;
  bool DisplaySleep;
  bool MirrorToWindow;
  bool PixelLuminanceOverdrive;
  bool TimewarpEnabled;
  bool TimewarpNoJitEnabled;
  bool Multisample;
  bool PositionTrackingEnabled;
};

class OculusVr::OculusVrPimpl {
 private:
   ovrHmd      Hmd;
   ovrSizei WindowSize;
   ovrFovPort eyeFov[2];
   ovrSizei EyeRenderSize[2];
   ovrEyeRenderDesc EyeRenderDesc[2];
   ovrGLConfig cfg;
   ovrMatrix4f Projection[2];

 public:
  OculusVrPimpl() {
    assert(this);

    Hmd = ovrHmd_Create(0);
    if (!Hmd) {
      // If we didn't detect an Hmd, create a simulated one for debugging.
      Hmd = ovrHmd_CreateDebug(ovrHmd_DK1);
      if (!Hmd) {
        assert(false);
        throw "Failed Hmd creation";
      }
    }

    if (Hmd->HmdCaps & ovrHmdCap_ExtendDesktop) {
      WindowSize = Hmd->Resolution;
    }
    else {
      // In Direct App-rendered mode, we can use smaller window size,
      // as it can have its own contents and isn't tied to the buffer.
      // Sizei(960, 540); avoid rotated output bug.
      WindowSize.w = 1100;
      WindowSize.w = 618;
    }

    eyeFov[0] = Hmd->DefaultEyeFov[0];
    eyeFov[1] = Hmd->DefaultEyeFov[1];

    OculusSettings settings;

    // Configure Stereo settings. Default pixel density is 1.0f.
    float DesiredPixelDensity = 1.0f;
    ovrSizei recommenedTex0Size = ovrHmd_GetFovTextureSize(Hmd, ovrEye_Left,  eyeFov[0], DesiredPixelDensity);
    ovrSizei recommenedTex1Size = ovrHmd_GetFovTextureSize(Hmd, ovrEye_Right, eyeFov[1], DesiredPixelDensity);

    ovrSizei tex0Size = EnsureRendertargetAtLeastThisBig(Rendertarget_Left,  recommenedTex0Size);
    ovrSizei tex1Size = EnsureRendertargetAtLeastThisBig(Rendertarget_Right, recommenedTex1Size);

    EyeRenderSize[0] = SizeiMin(tex0Size, recommenedTex0Size);
    EyeRenderSize[1] = SizeiMin(tex1Size, recommenedTex1Size);

    unsigned hmdCaps = (settings.VsyncEnabled ? 0 : ovrHmdCap_NoVSync);
    if (settings.IsLowPersistence)
      hmdCaps |= ovrHmdCap_LowPersistence;

    // ovrHmdCap_DynamicPrediction - enables internal latency feedback
    if (settings.DynamicPrediction)
      hmdCaps |= ovrHmdCap_DynamicPrediction;

    // ovrHmdCap_DisplayOff - turns off the display
    if (settings.DisplaySleep)
      hmdCaps |= ovrHmdCap_DisplayOff;

    if (!settings.MirrorToWindow)
      hmdCaps |= ovrHmdCap_NoMirrorToWindow;

    ovrHmd_SetEnabledCaps(Hmd, hmdCaps);

    
    cfg.OGL.Header.API         = ovrRenderAPI_OpenGL;
    cfg.OGL.Header.RTSize      = WindowSize;
    cfg.OGL.Header.Multisample = settings.Multisample;

    // The optional window handle. If unset, rendering will use the current window.
    // cfg.OGL.Window             = Window;

    ovrRenderAPIConfig config         = cfg.Config;
    unsigned           distortionCaps = ovrDistortionCap_Chromatic |
      ovrDistortionCap_Vignette |
      ovrDistortionCap_SRGB;
    if(settings.PixelLuminanceOverdrive)
      distortionCaps |= ovrDistortionCap_Overdrive;
    if (settings.TimewarpEnabled)
      distortionCaps |= ovrDistortionCap_TimeWarp;
    if(settings.TimewarpNoJitEnabled)
      distortionCaps |= ovrDistortionCap_ProfileNoTimewarpSpinWaits;

    if (!ovrHmd_ConfigureRendering( Hmd, &config, distortionCaps,
      eyeFov, EyeRenderDesc ))
    {
      // Fail exit? TBD
      return;
    }

    unsigned sensorCaps = ovrTrackingCap_Orientation|ovrTrackingCap_MagYawCorrection;
    if (settings.PositionTrackingEnabled)
      sensorCaps |= ovrTrackingCap_Position;
    ovrHmd_ConfigureTracking(Hmd, sensorCaps, 0);

    // Calculate projections
    Projection[0] = ovrMatrix4f_Projection(EyeRenderDesc[0].Fov,  0.01f, 10000.0f, true);
    Projection[1] = ovrMatrix4f_Projection(EyeRenderDesc[1].Fov,  0.01f, 10000.0f, true);

    /*
    float    orthoDistance = 0.8f; // 2D is 0.8 meter from camera
    Vector2f orthoScale0   = Vector2f(1.0f) / Vector2f(EyeRenderDesc[0].PixelsPerTanAngleAtCenter);
    Vector2f orthoScale1   = Vector2f(1.0f) / Vector2f(EyeRenderDesc[1].PixelsPerTanAngleAtCenter);
    OrthoProjection[0] = ovrMatrix4f_OrthoSubProjection(Projection[0], orthoScale0, orthoDistance,
      EyeRenderDesc[0].ViewAdjust.x);
    OrthoProjection[1] = ovrMatrix4f_OrthoSubProjection(Projection[1], orthoScale1, orthoDistance,
      EyeRenderDesc[1].ViewAdjust.x);
    */
  }
  ~OculusVrPimpl() {
    assert(this);
    ovrHmd_Destroy(Hmd);
  }
};

OculusVr::OculusVr() {
  assert(this);
  ovr_Initialize();
  pimpl_.reset(new OculusVrPimpl());
}

OculusVr::~OculusVr() {
  assert(this);
  pimpl_.reset();
  ovrHmd_Destroy();
}

#if 0

const EyeSetup OculusVr::LeftEye() {
  assert(this);
  return pimpl_->GetEyeSetup(OVR::Util::Render::StereoEye_Left);
}

const EyeSetup OculusVr::RightEye() {
  assert(this);
  return pimpl_->GetEyeSetup(OVR::Util::Render::StereoEye_Right);
}

const Vec4& OculusVr::GetDistortion() const {
  assert(this);
  return pimpl_->distortion();
}

float OculusVr::GetScale() const {
  assert(this);
  return pimpl_->render_scale();
}

Quat OculusVr::GetOrientation(bool predict_orientation) const {
  assert(this);
  return pimpl_->CalculateOrientation(predict_orientation);
}

void OculusVr::ResetOrientation() {
  assert(this);
  return pimpl_->ResetOrientation();
}

const Vec2& OculusVr::GetCenterOffset() const {
  assert(this);
  return pimpl_->center_offset();
}

const Vec4 OculusVr::GetChromaticAberration() const {
  assert(this);
  return pimpl_->chromatic_aberration();
}

const std::string OculusVr::GetDetectionMessage() const {
  assert(this);
  return pimpl_->GetDetectionMessage();
}


#endif