  // Euphoria - Copyright (c) Gustav
// Euphoria is awesome: 幸福感是真棒

#include "euphoria/oculusvr.h"

#include <cassert>

#include "OVR.h"  // NOLINT this is how you should include OVR

struct OculusVr::OculusVrPimpl {
  OVR::Ptr<OVR::DeviceManager> device_manager_;
  OVR::Ptr<OVR::HMDDevice> device_;
  OVR::Util::Render::StereoConfig stereo_config_;
  OVR::HMDInfo device_info_;
  float render_scale_;

  OculusVrPimpl() {
    /// @todo use correct resolution when creating the stereo
    int Width = 100;
    int Height = 100;

    OVR::System::Init(OVR::Log::ConfigureDefaultLog(OVR::LogMask_All));

    device_manager_ = *OVR::DeviceManager::Create();
    device_ = *device_manager_->EnumerateDevices<OVR::HMDDevice>()
              .CreateDevice();
    if (device_.GetPtr() != NULL) {
      device_->GetDeviceInfo(&device_info_);
    } else {
      // no device is connected, setup dummy values for testing
      // without a oculus connected.
      device_info_.Type = OVR::Device_HMD;
      strncpy(device_info_.ProductName, "Oculus Rift DK1",
              OVR::DeviceInfo::MaxNameLength);
      strncpy(device_info_.Manufacturer, "Oculus VR",
              OVR::DeviceInfo::MaxNameLength);
      device_info_.Version = 0;

      device_info_.HResolution = 1280;
      device_info_.VResolution = 800;
      device_info_.HScreenSize = 0.149759993f;
      device_info_.VScreenSize = 0.0935999975f;
      device_info_.VScreenCenter = 0.0467999987f;
      device_info_.EyeToScreenDistance = 0.0410000011f;
      device_info_.LensSeparationDistance = 0.0635000020f;
      device_info_.InterpupillaryDistance = 0.0625000000f;
      device_info_.DesktopX = -1280;
      device_info_.DesktopY = 480;
      device_info_.DisplayId = 0;

      device_info_.DistortionK[0] = 1.00000000f;
      device_info_.DistortionK[1] = 0.219999999f;
      device_info_.DistortionK[2] = 0.239999995f;
      device_info_.DistortionK[3] = 0.000000000f;
      device_info_.ChromaAbCorrection[0] = 0.995999992f;
      device_info_.ChromaAbCorrection[1] = -0.00400000019f;
      device_info_.ChromaAbCorrection[2] = 1.01400006f;
      device_info_.ChromaAbCorrection[3] = 0.000000000f;
      strncpy(device_info_.DisplayDeviceName,
              "\\\\.\\DISPLAY3\\Monitor0", 32);
    }
    stereo_config_.SetFullViewport(OVR::Util::Render::Viewport(0, 0,
                                   Width, Height));
    stereo_config_.SetStereoMode(
      OVR::Util::Render::Stereo_LeftRight_Multipass);
    stereo_config_.SetHMDInfo(device_info_);
    stereo_config_.SetDistortionFitPointVP(-1.0f, 0.0f);
    render_scale_ = stereo_config_.GetDistortionScale();
  }

  ~OculusVrPimpl() {
  }

  const EyeSetup Eye(OVR::Util::Render::StereoEye eyeid) {
    const OVR::Util::Render::StereoEyeParams leftEye
      = stereo_config_.GetEyeRenderParams(eyeid);
    // Left eye rendering parameters
    auto leftVP = leftEye.VP;
    auto leftProjection = leftEye.Projection;
    auto leftViewAdjust = leftEye.ViewAdjust;
    return EyeSetup();
  }
};

OculusVr::OculusVr() : pimpl_(new OculusVrPimpl()) {
}

OculusVr::~OculusVr() {
}

const EyeSetup OculusVr::LeftEye() {
  return pimpl_->Eye(OVR::Util::Render::StereoEye_Left);
}

const EyeSetup OculusVr::RightEye() {
  return pimpl_->Eye(OVR::Util::Render::StereoEye_Right);
}
