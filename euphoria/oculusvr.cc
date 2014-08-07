// Euphoria - Copyright (c) Gustav

#include "euphoria/oculusvr.h"

#include <cassert>
#include <string>

#include "euphoria/texture.h"

#include "OVR.h"  // NOLINT this is how you should include OVR
#include "../Src/OVR_CAPI_GL.h"

EyeSetup::EyeSetup(const Mat44& projection, const Mat44& view_adjust, int w, int h, bool mipmap)
    : projection_(projection),
      view_adjust_(view_adjust), fbo_(w, h, mipmap) {}

const Mat44& EyeSetup::projection() const {
  assert(this);
  return projection_;
}

void EyeSetup::set_projection(const Mat44& projection) {
  assert(this);
  projection_ = projection;
}

const Mat44& EyeSetup::view_adjust() const {
  assert(this);
  return view_adjust_;
}

void EyeSetup::set_view_adjust(const Mat44& view_adjust) {
  assert(this);
  view_adjust_ = view_adjust;
}

Fbo& EyeSetup::fbo() {
  assert(this);
  return fbo_;
}

const Fbo& EyeSetup::fbo() const {
  assert(this);
  return fbo_;
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
ovrSizei SizeiMax(ovrSizei a, ovrSizei b) {
  ovrSizei ret;
  ret.w = (a.w  > b.w)  ? a.w  : b.w;
  ret.h = (a.h > b.h) ? a.h : b.h;
  return ret;
}

struct OculusSettings {
  OculusSettings()
    : VsyncEnabled(false),
    IsLowPersistence(false),
    DynamicPrediction(false),
    DisplaySleep(false),
    MirrorToWindow(false),
    PixelLuminanceOverdrive(false),
    TimewarpEnabled(false),
    TimewarpNoJitEnabled(false),
    Multisample(false),
    PositionTrackingEnabled(false),
    MipMapEyeTextures(false)
  {
  }

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
  bool MipMapEyeTextures;
};

ovrSizei Sizei(int h, int w) {
  ovrSizei ret;
  ret.h = h;
  ret.w = w;
  return ret;
}
ovrSizei Sizei(int i) {
  ovrSizei ret;
  ret.h = ret.w = i;
  return ret;
}

ovrSizei EnsureRendertargetAtLeastThisBig(ovrEyeType eye, ovrSizei ret) {
  ovrSizei newRTSize = ret;
  // Put some sane limits on it. 4k x 4k is fine for most modern video cards.
  // Nobody should be messing around with surfaces smaller than 4k pixels these days.
  newRTSize = SizeiMax(SizeiMin(newRTSize, Sizei(4096)), Sizei(64));
  return ret;
}

ovrHmd DetectHmdOrNull(bool detect_debug_device) {
  ovrHmd Hmd = 0;

  Hmd = ovrHmd_Create(0);
  if (!Hmd) {
    if( detect_debug_device ) {
      // If we didn't detect an Hmd, create a simulated one for debugging.
      Hmd = ovrHmd_CreateDebug(ovrHmd_DK1);
      if (!Hmd) {
        assert(false);
        throw "Failed to create OVR debug device";
      }
    }
  }

  return Hmd;
}

ovrRecti Recti(const ovrSizei& s) {
  ovrRecti r;
  r.Pos.x = 0;
  r.Pos.y = 0;
  r.Size = s;
  return r;
}

ovrTexture GetOvrTexture(const EyeSetup& eye) {
  ovrTexture tex;

  ovrSizei newRTSize = Sizei(eye.fbo().width(), eye.fbo().height());

  ovrGLTextureData* texData = (ovrGLTextureData*)&tex;
  texData->Header.API            = ovrRenderAPI_OpenGL;
  texData->Header.TextureSize    = newRTSize;
  texData->Header.RenderViewport = Recti(newRTSize);
  texData->TexId                 = eye.fbo().texture().texture();

  return tex;
}

enum {
  INDEX_LEFT = 0,
  INDEX_RIGHT = 1
};

class OculusVr::OculusVrPimpl {
 private:
   ovrHmd hmd_;
   ovrSizei window_size_;
   ovrFovPort eye_fov_[2];
   ovrSizei eye_render_size_[2];
   ovrEyeRenderDesc eye_render_desc_[2];
   ovrGLConfig cfg_;
   ovrPosef poses_[2];
   ovrTexture textures_[2];
   std::unique_ptr<EyeSetup> left_eye_;
   std::unique_ptr<EyeSetup> right_eye_;

 public:
  OculusVrPimpl(ovrHmd hmd) : hmd_(hmd) {
    assert(this);
    assert(hmd);

    if (hmd_->HmdCaps & ovrHmdCap_ExtendDesktop) {
      window_size_ = hmd_->Resolution;
    }
    else {
      // In Direct App-rendered mode, we can use smaller window size,
      // as it can have its own contents and isn't tied to the buffer.
      // Sizei(960, 540); avoid rotated output bug.
      window_size_.w = 1100;
      window_size_.w = 618;
    }

    eye_fov_[INDEX_LEFT] = hmd_->DefaultEyeFov[INDEX_LEFT];
    eye_fov_[INDEX_RIGHT] = hmd_->DefaultEyeFov[INDEX_RIGHT];

    OculusSettings settings;

    // Configure Stereo settings. Default pixel density is 1.0f.
    float DesiredPixelDensity = 1.0f;
    ovrSizei recommenedTexleftSize = ovrHmd_GetFovTextureSize(hmd_, ovrEye_Left,  eye_fov_[INDEX_LEFT], DesiredPixelDensity);
    ovrSizei recommenedTexRightSize = ovrHmd_GetFovTextureSize(hmd_, ovrEye_Right, eye_fov_[INDEX_RIGHT], DesiredPixelDensity);

    ovrSizei texLeftSize = EnsureRendertargetAtLeastThisBig(ovrEye_Left,  recommenedTexleftSize);
    ovrSizei texRightSize = EnsureRendertargetAtLeastThisBig(ovrEye_Right, recommenedTexRightSize);

    // setup eye textures and stuff
    left_eye_.reset( new EyeSetup(CreateIdentityMat44(), CreateIdentityMat44(), texLeftSize.w, texLeftSize.h, settings.MipMapEyeTextures));
    right_eye_.reset( new EyeSetup(CreateIdentityMat44(), CreateIdentityMat44(), texRightSize.w, texRightSize.h, settings.MipMapEyeTextures));

    eye_render_size_[INDEX_LEFT] = SizeiMin(texLeftSize, recommenedTexleftSize);
    eye_render_size_[INDEX_RIGHT] = SizeiMin(texRightSize, recommenedTexRightSize);

    // Store texture pointers and viewports that will be passed for rendering.
    textures_[0]                       = GetOvrTexture(*left_eye_);
    textures_[0].Header.TextureSize    = texLeftSize;
    textures_[0].Header.RenderViewport = Recti(eye_render_size_[INDEX_LEFT]);
    textures_[1]                       = GetOvrTexture(*right_eye_);
    textures_[1].Header.TextureSize    = texRightSize;
    textures_[1].Header.RenderViewport = Recti(eye_render_size_[INDEX_RIGHT]);

    unsigned hmd_caps = (settings.VsyncEnabled ? 0 : ovrHmdCap_NoVSync);
    if (settings.IsLowPersistence)
      hmd_caps |= ovrHmdCap_LowPersistence;

    // ovrHmdCap_DynamicPrediction - enables internal latency feedback
    if (settings.DynamicPrediction)
      hmd_caps |= ovrHmdCap_DynamicPrediction;

    // ovrHmdCap_DisplayOff - turns off the display
    if (settings.DisplaySleep)
      hmd_caps |= ovrHmdCap_DisplayOff;

    if (!settings.MirrorToWindow)
      hmd_caps |= ovrHmdCap_NoMirrorToWindow;

    ovrHmd_SetEnabledCaps(hmd_, hmd_caps);

    cfg_.OGL.Header.API         = ovrRenderAPI_OpenGL;
    cfg_.OGL.Header.RTSize      = window_size_;
    cfg_.OGL.Header.Multisample = settings.Multisample;

    // The optional window handle. If unset, rendering will use the current window.
    // cfg.OGL.Window = Window;

    ovrRenderAPIConfig config = cfg_.Config;
    unsigned distortionCaps = ovrDistortionCap_Chromatic |
      ovrDistortionCap_Vignette |
      ovrDistortionCap_SRGB;
    if(settings.PixelLuminanceOverdrive)
      distortionCaps |= ovrDistortionCap_Overdrive;
    if (settings.TimewarpEnabled)
      distortionCaps |= ovrDistortionCap_TimeWarp;
    if(settings.TimewarpNoJitEnabled)
      distortionCaps |= ovrDistortionCap_ProfileNoTimewarpSpinWaits;

    if (!ovrHmd_ConfigureRendering( hmd_, &config, distortionCaps,
      eye_fov_, eye_render_desc_ ))
    {
      // Fail exit? TBD
      throw "Failed to configure oculus rendering";
    }

    unsigned sensorCaps = ovrTrackingCap_Orientation|ovrTrackingCap_MagYawCorrection;
    if (settings.PositionTrackingEnabled)
      sensorCaps |= ovrTrackingCap_Position;
    ovrHmd_ConfigureTracking(hmd_, sensorCaps, 0);

    // Calculate projections
    const ovrMatrix4f projection_left = ovrMatrix4f_Projection(eye_render_desc_[INDEX_LEFT].Fov,  0.01f, 10000.0f, true);
    const ovrMatrix4f projection_right = ovrMatrix4f_Projection(eye_render_desc_[INDEX_RIGHT].Fov,  0.01f, 10000.0f, true);
    
    left_eye_->set_projection(C(projection_left));
    right_eye_->set_projection(C(projection_right));

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
    ovrHmd_Destroy(hmd_);
  }

  void begin() {
    ovrHmd_BeginFrame(hmd_, 0);

    for (int eyeIndex = 0; eyeIndex < ovrEye_Count; eyeIndex++)
    {      
      ovrEyeType eye = hmd_->EyeRenderOrder[eyeIndex];
      ovrPosef pose = ovrHmd_GetEyePose(hmd_, eye);
      poses_[eye] = pose;
      // view = CalculateViewFromPose(pose);
    }
  }

  void end() {
    ovrHmd_EndFrame(hmd_, poses_, textures_);
  }
};

OculusVr::OculusVr() {
  assert(this);
  ovr_Initialize();
}

bool OculusVr::Detect(bool detect_debug_device) {
  assert(this);
  ovrHmd hmd = DetectHmdOrNull(detect_debug_device);
  if( hmd ) {
    pimpl_.reset(new OculusVrPimpl(hmd));
    return true;
  }
  else {
    return false;
  }
}

OculusVr::~OculusVr() {
  assert(this);
  pimpl_.reset();
  ovr_Shutdown();
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