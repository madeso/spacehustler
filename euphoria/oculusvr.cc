﻿  // Euphoria - Copyright (c) Gustav
// Euphoria is awesome: 幸福感是真棒

#include "euphoria/oculusvr.h"

#include <cassert>

#include "OVR.h"  // NOLINT this is how you should include OVR

EyeSetup::EyeSetup(float w, float h, float x, float y, const mat44& projection,
                   const mat44& view_adjust)
  : w_(w), h_(h), x_(x), y_(y)
  , projection_(projection), view_adjust_(view_adjust) {
}

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
const mat44& EyeSetup::projection() const {
  assert(this);
  return projection_;
}
const mat44& EyeSetup::view_adjust() const {
  assert(this);
  return view_adjust_;
}

mat44 C(OVR::Matrix4f m) {
  // matrix4f is row-major
  // mat44 is col major
  /// @todo this seems wrong, but it works out for the better, perhaps the
  /// viewmatrix should be transposed too, so we need to transpose twice?
  return mat44(m.M);
  // return cml::transpose(mat44(m.M));
}

void SetupDummyValues(OVR::HMDInfo* device_info) {
  assert(device_info);

  device_info->Type = OVR::Device_HMD;
  strncpy(device_info->ProductName, "Oculus Rift DK1",
          OVR::DeviceInfo::MaxNameLength);
  strncpy(device_info->Manufacturer, "Oculus VR",
          OVR::DeviceInfo::MaxNameLength);
  device_info->Version = 0;

  device_info->HResolution = 1280;
  device_info->VResolution = 800;
  device_info->HScreenSize = 0.149759993f;
  device_info->VScreenSize = 0.0935999975f;
  device_info->VScreenCenter = 0.0467999987f;
  device_info->EyeToScreenDistance = 0.0410000011f;
  device_info->LensSeparationDistance = 0.0635000020f;
  device_info->InterpupillaryDistance = 0.0625000000f;
  device_info->DesktopX = -1280;
  device_info->DesktopY = 480;
  device_info->DisplayId = 0;
  device_info->DistortionK[0] = 1.00000000f;
  device_info->DistortionK[1] = 0.219999999f;
  device_info->DistortionK[2] = 0.239999995f;
  device_info->DistortionK[3] = 0.000000000f;
  device_info->ChromaAbCorrection[0] = 0.995999992f;
  device_info->ChromaAbCorrection[1] = -0.00400000019f;
  device_info->ChromaAbCorrection[2] = 1.01400006f;
  device_info->ChromaAbCorrection[3] = 0.000000000f;
  strncpy(device_info->DisplayDeviceName,
          "\\\\.\\DISPLAY3\\Monitor0", 32);
}

struct OculusVr::OculusVrPimpl {
  OVR::Ptr<OVR::DeviceManager> device_manager_;
  OVR::Ptr<OVR::HMDDevice> device_;
  OVR::Util::Render::StereoConfig stereo_config_;
  OVR::HMDInfo device_info_;
  float render_scale_;
  vec4 distortion_;
  vec2 center_offset_;
  vec4 chromatic_aberration_;

  OculusVrPimpl() : render_scale_(0.0f) , distortion_(0.0f, 0.0f, 0.0f, 0.0f),
    center_offset_(0.0f, 0.0f), chromatic_aberration_(0.0f, 0.0f, 0.0f, 0.0f) {
    assert(this);
    /// @todo use correct resolution when creating the stereo
    device_manager_ = *OVR::DeviceManager::Create();
    device_ = *device_manager_->EnumerateDevices<OVR::HMDDevice>()
              .CreateDevice();
    if (device_.GetPtr() != NULL) {
      device_->GetDeviceInfo(&device_info_);
    } else {
      // no device is connected, setup dummy values for testing
      // without a oculus connected.
      SetupDummyValues(&device_info_);
    }

    const int Width = device_info_.HResolution;
    const int Height = device_info_.VResolution;

    stereo_config_.SetFullViewport(OVR::Util::Render::Viewport(0, 0,
                                   Width, Height));
    stereo_config_.SetStereoMode(
      OVR::Util::Render::Stereo_LeftRight_Multipass);
    stereo_config_.SetHMDInfo(device_info_);
    stereo_config_.SetDistortionFitPointVP(-1.0f, 0.0f);

    const auto distortion = stereo_config_.GetDistortionConfig();

    center_offset_ = vec2(distortion.XCenterOffset,
                          distortion.YCenterOffset);

    render_scale_ = stereo_config_.GetDistortionScale();
    distortion_ = vec4(
                    device_info_.DistortionK[0], device_info_.DistortionK[1],
                    device_info_.DistortionK[2], device_info_.DistortionK[3]);

    chromatic_aberration_ = vec4(
                              distortion.ChromaticAberration[0],
                              distortion.ChromaticAberration[1],
                              distortion.ChromaticAberration[2],
                              distortion.ChromaticAberration[3]);
  }

  const vec4& get_distortion() const {
    assert(this);
    return distortion_;
  }

  float get_scale() const {
    assert(this);
    return render_scale_;
  }

  const vec2& get_center_offset() const {
    assert(this);
    return center_offset_;
  }

  const vec4 get_chromatic_aberration() const {
    assert(this);
    return chromatic_aberration_;
  }

  ~OculusVrPimpl() {
    assert(this);
  }

  const EyeSetup Eye(OVR::Util::Render::StereoEye eyeid) {
    assert(this);
    const OVR::Util::Render::StereoEyeParams leftEye
      = stereo_config_.GetEyeRenderParams(eyeid);
    // Left eye rendering parameters
    auto vp = leftEye.VP;

    const float Width = 1.0f;  // device_info_.HResolution;
    const float Height = 1.0f;  // device_info_.VResolution;

    auto projection = leftEye.Projection;
    auto viewAdjust = leftEye.ViewAdjust;
    return EyeSetup(vp.w / Width, vp.h / Height, vp.x / Width, vp.y / Height,
                    C(projection), C(viewAdjust));
  }
};

OculusVr::OculusVr() {
  assert(this);
  OVR::System::Init(OVR::Log::ConfigureDefaultLog(OVR::LogMask_All));
  pimpl_.reset(new OculusVrPimpl());
}

OculusVr::~OculusVr() {
  assert(this);
  pimpl_.reset();
  OVR::System::Destroy();
}

const EyeSetup OculusVr::LeftEye() {
  assert(this);
  return pimpl_->Eye(OVR::Util::Render::StereoEye_Left);
}

const EyeSetup OculusVr::RightEye() {
  assert(this);
  return pimpl_->Eye(OVR::Util::Render::StereoEye_Right);
}

const vec4& OculusVr::get_distortion() const {
  assert(this);
  return pimpl_->get_distortion();
}

float OculusVr::get_scale() const {
  assert(this);
  return pimpl_->get_scale();
}

const vec2& OculusVr::get_center_offset() const {
  assert(this);
  return pimpl_->get_center_offset();
}

const vec4 OculusVr::get_chromatic_aberration() const {
  assert(this);
  return pimpl_->get_chromatic_aberration();
}
