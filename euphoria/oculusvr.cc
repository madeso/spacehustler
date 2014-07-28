// Euphoria - Copyright (c) Gustav

#include "euphoria/oculusvr.h"

#include <cassert>
#include <string>

#include "OVR.h"  // NOLINT this is how you should include OVR

EyeSetup::EyeSetup(float w, float h, float x, float y, const mat44& projection,
                   const mat44& view_adjust)
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
const mat44& EyeSetup::projection() const {
  assert(this);
  return projection_;
}
const mat44& EyeSetup::view_adjust() const {
  assert(this);
  return view_adjust_;
}

mat44 C(const OVR::Matrix4f& m) {
  // matrix4f is row-major
  // mat44 is col major
  /// @todo this seems wrong, but it works out for the better, perhaps the
  /// viewmatrix should be transposed too, so we need to transpose twice?
  return mat44(m.M);
  // return cml::transpose(mat44(m.M));
}

quat C(const OVR::Quatf& q) { return quat(q.x, q.y, q.z, q.w); }

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
  strncpy(device_info->DisplayDeviceName, "\\\\.\\DISPLAY3\\Monitor0", 32);
}

struct OculusVr::OculusVrPimpl {
  OVR::Ptr<OVR::DeviceManager> device_manager;
  OVR::Ptr<OVR::HMDDevice> device;
  OVR::Util::Render::StereoConfig stereo_config;
  OVR::HMDInfo device_info;
  float render_scale;
  vec4 distortion;
  vec2 center_offset;
  vec4 chromatic_aberration;

  OVR::Ptr<OVR::SensorDevice> sensor;
  OVR::SensorFusion sensor_fusion;

  OculusVrPimpl()
      : render_scale(0.0f),
        distortion(0.0f, 0.0f, 0.0f, 0.0f),
        center_offset(0.0f, 0.0f),
        chromatic_aberration(0.0f, 0.0f, 0.0f, 0.0f) {
    assert(this);

    device_manager = *OVR::DeviceManager::Create();
    device = *device_manager->EnumerateDevices<OVR::HMDDevice>().CreateDevice();

    // Rendering setup:

    if (device.GetPtr() != NULL) {
      device->GetDeviceInfo(&device_info);
      stereo_config.SetHMDInfo(device_info);
    } else {
      // no device is connected, setup dummy values for testing
      // without a oculus connected.
      SetupDummyValues(&device_info);
    }

    /// @todo use correct resolution when creating the stereo
    const int Width = device_info.HResolution;
    const int Height = device_info.VResolution;

    stereo_config.SetFullViewport(
        OVR::Util::Render::Viewport(0, 0, Width, Height));
    stereo_config.SetStereoMode(OVR::Util::Render::Stereo_LeftRight_Multipass);
    stereo_config.SetDistortionFitPointVP(-1.0f, 0.0f);

    const auto distortion = stereo_config.GetDistortionConfig();

    center_offset = vec2(distortion.XCenterOffset, distortion.YCenterOffset);

    render_scale = stereo_config.GetDistortionScale();
    this->distortion =
        vec4(device_info.DistortionK[0], device_info.DistortionK[1],
             device_info.DistortionK[2], device_info.DistortionK[3]);

    chromatic_aberration = vec4(
        distortion.ChromaticAberration[0], distortion.ChromaticAberration[1],
        distortion.ChromaticAberration[2], distortion.ChromaticAberration[3]);

    // Input setup:
    if (device.GetPtr() != NULL) {
      sensor = device->GetSensor();
    }

    if (sensor) {
      sensor_fusion.AttachToSensor(sensor);
    }
  }

  const vec4& get_distortion() const {
    assert(this);
    return distortion;
  }

  float get_scale() const {
    assert(this);
    return render_scale;
  }

  quat get_orientation(bool predict_orientation) const {
    if (predict_orientation) {
      return C(sensor_fusion.GetPredictedOrientation());
    } else {
      return C(sensor_fusion.GetOrientation());
    }
  }

  void reset_orientation() { sensor_fusion.Reset(); }

  const vec2& get_center_offset() const {
    assert(this);
    return center_offset;
  }

  const vec4 get_chromatic_aberration() const {
    assert(this);
    return chromatic_aberration;
  }

  const std::string detection_message() const {
    if (!device && !sensor) {
      return "Oculus Rift not detected.";
    } else if (!device) {
      return "Oculus Sensor detected; HMD Display not detected.";
    } else if (!sensor) {
      return "Oculus HMD Display detected; Sensor not detected.";
    } else if (device_info.DisplayDeviceName[0] == '\0') {
      return "Oculus Sensor detected; HMD display EDID not detected.";
    } else {
      return "";
    }
  }

  ~OculusVrPimpl() { assert(this); }

  const EyeSetup Eye(OVR::Util::Render::StereoEye eyeid) {
    assert(this);
    const OVR::Util::Render::StereoEyeParams leftEye =
        stereo_config.GetEyeRenderParams(eyeid);
    // Left eye rendering parameters
    auto vp = leftEye.VP;

    const float Width = 1.0f;   // device_info_.HResolution;
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

const vec4& OculusVr::GetDistortion() const {
  assert(this);
  return pimpl_->get_distortion();
}

float OculusVr::GetScale() const {
  assert(this);
  return pimpl_->get_scale();
}

quat OculusVr::GetOrientation(bool predict_orientation) const {
  assert(this);
  return pimpl_->get_orientation(predict_orientation);
}

void OculusVr::ResetOrientation() {
  assert(this);
  return pimpl_->reset_orientation();
}

const vec2& OculusVr::GetCenterOffset() const {
  assert(this);
  return pimpl_->get_center_offset();
}

const vec4 OculusVr::GetChromaticAberration() const {
  assert(this);
  return pimpl_->get_chromatic_aberration();
}

const std::string OculusVr::GetDetectionMessage() const {
  assert(this);
  return pimpl_->detection_message();
}
