// Euphoria - Copyright (c) Gustav

#include "euphoria/oculusvr.h"

#include <cassert>
#include <string>

#include "euphoria/texture.h"
#include "euphoria/log.h"
#include "euphoria/settings.h"

#include "OVR.h"  // NOLINT this is how you should include OVR
#include "../Src/OVR_CAPI_GL.h"

namespace euphoria {

EyeSetup::EyeSetup(const Mat44& projection, const Mat44& view_adjust, int w,
                   int h, bool mipmap)
    : projection_(projection), view_adjust_(view_adjust), fbo_(w, h, mipmap) {}

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

Fbo* EyeSetup::GetFboPtr() {
  assert(this);
  return &fbo_;
}

Mat44 C(const OVR::Matrix4f& m) {
  // matrix4f is row-major
  // mat44 is col major
  /// @todo this seems wrong, but it works out for the better, perhaps the
  /// viewmatrix should be transposed too, so we need to transpose twice?
  return Mat44(m.M);
  // return cml::transpose(Mat44(m.M));
}

Vec3 C(const ovrVector3f& vec) { return 25 * Vec3(-vec.x, -vec.y, -vec.z); }

Quat C(const ovrQuatf& q) { return Quat(Vec3(-q.x, -q.y, -q.z), q.w); }

ovrSizei Sizei(int w, int h) {
  ovrSizei ret;
  ret.w = w;
  ret.h = h;
  return ret;
}

ovrSizei Sizei(int i) {
  ovrSizei ret;
  ret.h = ret.w = i;
  return ret;
}

ovrVector2i Vector2i(int x, int y) {
  ovrVector2i v;
  v.x = x;
  v.y = y;
  return v;
}

ovrVector2i Vector2i(int xy) { return Vector2i(xy, xy); }

const bool PROJECTION_RIGHT_HANDED = true;

const Mat44 PoseToMatrix(const ovrPosef& pose) {
  return CreateMat44(C(pose.Position), C(pose.Orientation));
}

ovrSizei SizeiMin(ovrSizei a, ovrSizei b) {
  ovrSizei ret;
  ret.w = (a.w < b.w) ? a.w : b.w;
  ret.h = (a.h < b.h) ? a.h : b.h;
  return ret;
}
ovrSizei SizeiMax(ovrSizei a, ovrSizei b) {
  ovrSizei ret;
  ret.w = (a.w > b.w) ? a.w : b.w;
  ret.h = (a.h > b.h) ? a.h : b.h;
  return ret;
}

ovrSizei EnsureRendertargetAtLeastThisBig(ovrEyeType eye, ovrSizei ret) {
  ovrSizei newRTSize = ret;
  // Put some sane limits on it. 4k x 4k is fine for most modern video cards.
  // Nobody should be messing around with surfaces smaller than 4k pixels these
  // days.
  newRTSize = SizeiMax(SizeiMin(newRTSize, Sizei(4096)), Sizei(64));
  return ret;
}

ovrHmd DetectHmdOrNull(bool detect_debug_device) {
  ovrHmd hmd_ = 0;

  hmd_ = ovrHmd_Create(0);
  if (!hmd_) {
    if (detect_debug_device) {
      // If we didn't detect an hmd_, create a simulated one for debugging.
      hmd_ = ovrHmd_CreateDebug(ovrHmd_DK1);
      if (!hmd_) {
        assert(false);
        throw "Failed to create OVR debug device";
      }
    }
  }

  return hmd_;
}

ovrRecti Recti(const ovrSizei& s) {
  ovrRecti r;
  r.Pos.x = 0;
  r.Pos.y = 0;
  r.Size = s;
  return r;
}

ovrRecti Recti(ovrVector2i v, const ovrSizei& s) {
  ovrRecti r;
  r.Pos.x = v.x;
  r.Pos.y = v.y;
  r.Size = s;
  return r;
}

template <typename T>
const T AlgMax(const T a, const T b) {
  return (b < a) ? a : b;
}
ovrFovPort FovPortMax(const ovrFovPort& a, const ovrFovPort& b) {
  ovrFovPort fov;
  fov.UpTan = AlgMax(a.UpTan, b.UpTan);
  fov.DownTan = AlgMax(a.DownTan, b.DownTan);
  fov.LeftTan = AlgMax(a.LeftTan, b.LeftTan);
  fov.RightTan = AlgMax(a.RightTan, b.RightTan);
  return fov;
}

ovrTexture GetOvrTexture(const EyeSetup& eye) {
  ovrTexture tex;

  ovrSizei newRTSize = ::euphoria::Sizei(eye.fbo().width(), eye.fbo().height());

  ovrGLTextureData* texData = reinterpret_cast<ovrGLTextureData*>(&tex);
  texData->Header.API = ovrRenderAPI_OpenGL;
  texData->Header.TextureSize = newRTSize;
  texData->Header.RenderViewport = Recti(newRTSize);
  texData->TexId = eye.fbo().texture().texture();

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
  int frameindex_;

 private:
  EyeSetup& GetEyeReferenceFromIndex(ovrEyeType eyeIndex) {
    assert(this);
    assert(left_eye_);
    assert(right_eye_);

    if (eyeIndex == ovrEye_Left) return *left_eye_;
    if (eyeIndex == ovrEye_Right) return *right_eye_;
    assert(false && "the index is neither left nor right");
    return *left_eye_;
  }

 public:
  OculusVrPimpl(const Settings& settings, ovrHmd hmd)
      : hmd_(hmd), frameindex_(0) {
    assert(this);
    assert(hmd);

    if (hmd_->HmdCaps & ovrHmdCap_ExtendDesktop) {
      window_size_ = hmd_->Resolution;
    } else {
      // In Direct App-rendered mode, we can use smaller window size,
      // as it can have its own contents and isn't tied to the buffer.
      // hmd_(960, 540); avoid rotated output bug.
      window_size_.w = 1100;
      window_size_.h = 618;
    }

    eye_fov_[INDEX_LEFT] = hmd_->DefaultEyeFov[INDEX_LEFT];
    eye_fov_[INDEX_RIGHT] = hmd_->DefaultEyeFov[INDEX_RIGHT];

    if (settings.force_zero_ipd()) {
      // ForceZeroIpd does three things:
      //  1) Sets FOV to maximum symmetrical FOV based on both eyes
      //  2) Sets eye ViewAdjust values to 0.0 (effective IPD == 0)
      //  3) Uses only the Left texture for rendering.

      eye_fov_[0] = FovPortMax(eye_fov_[0], eye_fov_[1]);
      eye_fov_[1] = eye_fov_[0];

      ovrSizei recommenedTexSize = ovrHmd_GetFovTextureSize(
          hmd_, ovrEye_Left, eye_fov_[0], settings.desired_pixel_density());

      ovrSizei textureSize =
          EnsureRendertargetAtLeastThisBig(ovrEye_Left, recommenedTexSize);

      left_eye_.reset(new EyeSetup(CreateIdentityMat44(), CreateIdentityMat44(),
                                   textureSize.w, textureSize.h,
                                   settings.mipmap_eye_textures()));

      eye_render_size_[0] = SizeiMin(textureSize, recommenedTexSize);
      eye_render_size_[1] = eye_render_size_[0];

      // Store texture pointers that will be passed for rendering.
      textures_[0] = GetOvrTexture(*left_eye_);
      textures_[0].Header.TextureSize = textureSize;
      textures_[0].Header.RenderViewport = Recti(eye_render_size_[0]);
      // Right eye is the same.
      textures_[1] = textures_[0];
    } else {
      // Configure Stereo settings. Default pixel density is 1.0f.
      float DesiredPixelDensity = 1.0f;
      ovrSizei recommenedTexleftSize = ovrHmd_GetFovTextureSize(
          hmd_, ovrEye_Left, eye_fov_[INDEX_LEFT], DesiredPixelDensity);
      ovrSizei recommenedTexRightSize = ovrHmd_GetFovTextureSize(
          hmd_, ovrEye_Right, eye_fov_[INDEX_RIGHT], DesiredPixelDensity);

      if (settings.rendertarget_is_shared_by_both_eyes()) {
        ovrSizei rtSize =
            Sizei(recommenedTexleftSize.w + recommenedTexRightSize.w,
                  AlgMax(recommenedTexleftSize.h, recommenedTexRightSize.h));

        // Use returned size as the actual RT size may be different due to HW
        // limits.
        rtSize = EnsureRendertargetAtLeastThisBig(ovrEye_Left, rtSize);

        left_eye_.reset(new EyeSetup(CreateIdentityMat44(),
                                     CreateIdentityMat44(), rtSize.w, rtSize.h,
                                     settings.mipmap_eye_textures()));

        // Don't draw more then recommended size; this also ensures that
        // resolution reported
        // in the overlay HUD size is updated correctly for FOV/pixel density
        // change.
        eye_render_size_[0] =
            SizeiMin(Sizei(rtSize.w / 2, rtSize.h), recommenedTexleftSize);
        eye_render_size_[1] =
            SizeiMin(Sizei(rtSize.w / 2, rtSize.h), recommenedTexRightSize);

        // Store texture pointers that will be passed for rendering.
        // Same texture is used, but with different viewports.
        textures_[0] = GetOvrTexture(*left_eye_);
        textures_[0].Header.TextureSize = rtSize;
        textures_[0].Header.RenderViewport =
            Recti(Vector2i(0), eye_render_size_[0]);
        textures_[1] = GetOvrTexture(*left_eye_);
        textures_[1].Header.TextureSize = rtSize;
        textures_[1].Header.RenderViewport =
            Recti(Vector2i((rtSize.w + 1) / 2, 0), eye_render_size_[1]);
      } else {
        ovrSizei texLeftSize = EnsureRendertargetAtLeastThisBig(
            ovrEye_Left, recommenedTexleftSize);
        ovrSizei texRightSize = EnsureRendertargetAtLeastThisBig(
            ovrEye_Right, recommenedTexRightSize);

        // setup eye textures and stuff
        left_eye_.reset(new EyeSetup(
            CreateIdentityMat44(), CreateIdentityMat44(), texLeftSize.w,
            texLeftSize.h, settings.mipmap_eye_textures()));
        right_eye_.reset(new EyeSetup(
            CreateIdentityMat44(), CreateIdentityMat44(), texRightSize.w,
            texRightSize.h, settings.mipmap_eye_textures()));

        eye_render_size_[INDEX_LEFT] =
            SizeiMin(texLeftSize, recommenedTexleftSize);
        eye_render_size_[INDEX_RIGHT] =
            SizeiMin(texRightSize, recommenedTexRightSize);

        // Store texture pointers and viewports that will be passed for
        // rendering.
        textures_[0] = GetOvrTexture(*left_eye_);
        textures_[0].Header.TextureSize = texLeftSize;
        textures_[0].Header.RenderViewport =
            Recti(eye_render_size_[INDEX_LEFT]);
        textures_[1] = GetOvrTexture(*right_eye_);
        textures_[1].Header.TextureSize = texRightSize;
        textures_[1].Header.RenderViewport =
            Recti(eye_render_size_[INDEX_RIGHT]);
      }
    }

    unsigned hmd_caps = (settings.vsync_enabled() ? 0 : ovrHmdCap_NoVSync);
    if (settings.is_low_persistence()) hmd_caps |= ovrHmdCap_LowPersistence;

    // ovrHmdCap_DynamicPrediction - enables internal latency feedback
    if (settings.dynamic_prediction()) hmd_caps |= ovrHmdCap_DynamicPrediction;

    // ovrHmdCap_DisplayOff - turns off the display
    if (settings.display_sleep()) hmd_caps |= ovrHmdCap_DisplayOff;

    if (!settings.mirror_to_window()) hmd_caps |= ovrHmdCap_NoMirrorToWindow;

    ovrHmd_SetEnabledCaps(hmd_, hmd_caps);

    cfg_.OGL.Header.API = ovrRenderAPI_OpenGL;
    cfg_.OGL.Header.RTSize = window_size_;
    cfg_.OGL.Header.Multisample = settings.multisample();

    // The optional window handle. If unset, rendering will use the current
    // window.
    // cfg.OGL.Window = Window;

    ovrRenderAPIConfig config = cfg_.Config;
    unsigned distortionCaps = ovrDistortionCap_Chromatic |
                              ovrDistortionCap_Vignette | ovrDistortionCap_SRGB;
    if (settings.pixel_luminance_overdrive())
      distortionCaps |= ovrDistortionCap_Overdrive;
    if (settings.timewarp_enabled())
      distortionCaps |= ovrDistortionCap_TimeWarp;
    if (settings.timewarp_no_jit_enabled())
      distortionCaps |= ovrDistortionCap_ProfileNoTimewarpSpinWaits;

    if (!ovrHmd_ConfigureRendering(hmd_, &config, distortionCaps, eye_fov_,
                                   eye_render_desc_)) {
      // Fail exit? TBD
      throw "Failed to configure oculus rendering";
    }

    unsigned sensorCaps =
        ovrTrackingCap_Orientation | ovrTrackingCap_MagYawCorrection;
    if (settings.position_tracking_enabled())
      sensorCaps |= ovrTrackingCap_Position;
    ovrHmd_ConfigureTracking(hmd_, sensorCaps, 0);

    // Calculate projections
    const ovrMatrix4f projection_left =
        ovrMatrix4f_Projection(eye_render_desc_[INDEX_LEFT].Fov, 0.01f,
                               10000.0f, PROJECTION_RIGHT_HANDED);
    const ovrMatrix4f projection_right =
        ovrMatrix4f_Projection(eye_render_desc_[INDEX_RIGHT].Fov, 0.01f,
                               10000.0f, PROJECTION_RIGHT_HANDED);

    left_eye_->set_projection(C(projection_left));
    right_eye_->set_projection(C(projection_right));
  }

  ~OculusVrPimpl() {
    assert(this);
    ovrHmd_Destroy(hmd_);
  }

  void Begin() {
    assert(this);
    ovrHmd_BeginFrame(hmd_, 0);

    ovrVector3f viewOffset[2] = {eye_render_desc_[0].HmdToEyeViewOffset,
                                 eye_render_desc_[1].HmdToEyeViewOffset};
    ovrHmd_GetEyePoses(hmd_, frameindex_, viewOffset, poses_, nullptr);
  }

  EyeSetup& GetEyeIndex(int eyeIndex) {
    assert(this);
    assert(eyeIndex >= 0 && eyeIndex < ovrEye_Count);
    ovrEyeType eye = hmd_->EyeRenderOrder[eyeIndex];

    EyeSetup& setup = GetEyeReferenceFromIndex(eye);
    setup.set_view_adjust(PoseToMatrix(poses_[eyeIndex]));
    // view = CalculateViewFromPose(pose);
    return setup;
  }

  void End() {
    assert(this);
    ovrHmd_EndFrame(hmd_, poses_, textures_);
    ++frameindex_;
  }

  const Vec2i window_size() const {
    assert(this);
    return Vec2i(window_size_.w, window_size_.h);
  }

  bool IsDisplayingWarning() const {
    assert(this);
    ovrHSWDisplayState display_state;
    ovrHmd_GetHSWDisplayState(hmd_, &display_state);
    bool displaying = display_state.Displayed != 0;

    return displaying;
  }

  void DismissWarning() {
    assert(this);
    ovrHSWDisplayState display_state;
    ovrHmd_GetHSWDisplayState(hmd_, &display_state);
    bool displaying = display_state.Displayed != 0;

    ovrBool res = ovrHmd_DismissHSWDisplay(hmd_);
    if (res) {
      LogInfo("Dismissed HSW display");
    } else {
      LogInfo("Unable to dismiss HSW display, dismissing queued");
      if (displaying) {
        double seconds = display_state.DismissibleTime - ovr_GetTimeInSeconds();
        if (seconds > 0) {
          LOGINFO(seconds << " until auto dismissing");
        }
      }
    }
  }
};

OculusVr::OculusVr() {
  assert(this);
  ovr_Initialize();
}

OculusVr::~OculusVr() {
  assert(this);
  pimpl_.reset();
  ovr_Shutdown();
}

bool OculusVr::Detect(const Settings& settings, bool detect_debug_device) {
  assert(this);
  ovrHmd hmd = DetectHmdOrNull(detect_debug_device);
  if (hmd) {
    pimpl_.reset(new OculusVrPimpl(settings, hmd));
    return true;
  } else {
    return false;
  }
}

bool OculusVr::IsHmdDetected() {
  assert(this);
  return pimpl_.get() != 0;
}

const Vec2i OculusVr::GetWindowSize() const {
  assert(this);
  assert(pimpl_ && "hmd must be detected");
  return pimpl_->window_size();
}

void OculusVr::Begin() {
  assert(this);
  assert(pimpl_ && "hmd must be detected");
  pimpl_->Begin();
}

int OculusVr::GetNumberOfEyes() const {
  assert(this);
  assert(pimpl_ && "hmd must be detected");
  return ovrEye_Count;
}

EyeSetup& OculusVr::GetEyeIndex(int eyeIndex) {
  assert(this);
  assert(pimpl_ && "hmd must be detected");
  return pimpl_->GetEyeIndex(eyeIndex);
}

void OculusVr::End() {
  assert(this);
  assert(pimpl_ && "hmd must be detected");
  pimpl_->End();
}

bool OculusVr::IsDisplayingWarning() const {
  assert(this);
  assert(pimpl_ && "hmd must be detected");
  return pimpl_->IsDisplayingWarning();
}

void OculusVr::DismissWarning() {
  assert(this);
  assert(pimpl_ && "hmd must be detected");
  pimpl_->DismissWarning();
}
}  // namespace euphoria
