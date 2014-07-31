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

Quat C(const OVR::Quatf& q) { return Quat(q.x, q.y, q.z, q.w); }

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

class OculusVr::OculusVrPimpl {
 private:
  OVR::Ptr<OVR::DeviceManager> device_manager_;
  OVR::Ptr<OVR::HMDDevice> device_;
  OVR::Util::Render::StereoConfig stereo_config_;
  OVR::HMDInfo device_info_;
  float render_scale_;
  Vec4 distortion_;
  Vec2 center_offset_;
  Vec4 chromatic_aberration_;

  OVR::Ptr<OVR::SensorDevice> sensor_;
  OVR::SensorFusion sensor_fusion_;

 public:
  OculusVrPimpl()
      : render_scale_(0.0f),
        distortion_(0.0f, 0.0f, 0.0f, 0.0f),
        center_offset_(0.0f, 0.0f),
        chromatic_aberration_(0.0f, 0.0f, 0.0f, 0.0f) {
    assert(this);

    device_manager_ = *OVR::DeviceManager::Create();
    device_ =
        *device_manager_->EnumerateDevices<OVR::HMDDevice>().CreateDevice();

    // Rendering setup:

    if (device_.GetPtr() != NULL) {
      device_->GetDeviceInfo(&device_info_);
      stereo_config_.SetHMDInfo(device_info_);
    } else {
      // no device is connected, setup dummy values for testing
      // without a oculus connected.
      SetupDummyValues(&device_info_);
    }

    /// @todo use correct resolution when creating the stereo
    const int Width = device_info_.HResolution;
    const int Height = device_info_.VResolution;

    stereo_config_.SetFullViewport(
        OVR::Util::Render::Viewport(0, 0, Width, Height));
    stereo_config_.SetStereoMode(OVR::Util::Render::Stereo_LeftRight_Multipass);
    stereo_config_.SetDistortionFitPointVP(-1.0f, 0.0f);

    const auto distortion = stereo_config_.GetDistortionConfig();

    center_offset_ = Vec2(distortion.XCenterOffset, distortion.YCenterOffset);

    render_scale_ = stereo_config_.GetDistortionScale();
    distortion_ =
        Vec4(device_info_.DistortionK[0], device_info_.DistortionK[1],
             device_info_.DistortionK[2], device_info_.DistortionK[3]);

    chromatic_aberration_ = Vec4(
        distortion.ChromaticAberration[0], distortion.ChromaticAberration[1],
        distortion.ChromaticAberration[2], distortion.ChromaticAberration[3]);

    // Input setup:
    if (device_.GetPtr() != NULL) {
      sensor_ = device_->GetSensor();
    }

    if (sensor_) {
      sensor_fusion_.AttachToSensor(sensor_);
    }
  }

  const Vec4& distortion() const {
    assert(this);
    return distortion_;
  }

  float render_scale() const {
    assert(this);
    return render_scale_;
  }

  Quat CalculateOrientation(bool predict_orientation) const {
    if (predict_orientation) {
      return C(sensor_fusion_.GetPredictedOrientation());
    } else {
      return C(sensor_fusion_.GetOrientation());
    }
  }

  void ResetOrientation() { sensor_fusion_.Reset(); }

  const Vec2& center_offset() const {
    assert(this);
    return center_offset_;
  }

  const Vec4 chromatic_aberration() const {
    assert(this);
    return chromatic_aberration_;
  }

  const std::string GetDetectionMessage() const {
    if (!device_ && !sensor_) {
      return "Oculus Rift not detected.";
    } else if (!device_) {
      return "Oculus Sensor detected; HMD Display not detected.";
    } else if (!sensor_) {
      return "Oculus HMD Display detected; Sensor not detected.";
    } else if (device_info_.DisplayDeviceName[0] == '\0') {
      return "Oculus Sensor detected; HMD display EDID not detected.";
    } else {
      return "";
    }
  }

  ~OculusVrPimpl() { assert(this); }

  const EyeSetup GetEyeSetup(OVR::Util::Render::StereoEye eyeid) {
    assert(this);
    const OVR::Util::Render::StereoEyeParams leftEye =
        stereo_config_.GetEyeRenderParams(eyeid);
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
