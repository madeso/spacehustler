// Euphoria - Copyright (c) Gustav

#include "euphoria/settings.h"

#include <cassert>
#include <fstream>  // NOLINT for loading data
#include <stdexcept>
#include <string>
#include <map>
#include "euphoria/str.h"
#include "euphoria/stringmerger.h"
#include "euphoria/stdutils.h"
#include "json/json.h"

namespace euphoria {

OculusVrDetection DetermineOculusVrValue(const std::string& name) {
  std::map<std::string, OculusVrDetection> values;
  values.insert(std::make_pair("auto", OculusVrDetection::AUTO));
  values.insert(std::make_pair("normal", OculusVrDetection::NORMAL));
  values.insert(std::make_pair("oculusvr", OculusVrDetection::OCULUS_VR));
  const auto res = values.find(name);
  if (res == values.end()) {
    throw std::logic_error(
        Str() << "Unknown oculus value: " << name << ", valid values are: "
              << StringMerger::EnglishAnd().Generate(Keys(values)));
  }
  return res->second;
}

Settings::Settings()
    : blackout_(false),
      width_(800),
      height_(600),
      fullscreen_(false),
      control_scheme_("keyboard"),
      primary_display_id_(0),
      oculus_vr_detection_(OculusVrDetection::AUTO),
      anisotropic_(1.0f),
      support_joystick_(true),
      vsync_enabled_(false),
      is_low_persistence_(true),
      dynamic_prediction_(true),
      display_sleep_(false),
      mirror_to_window_(true),
      pixel_luminance_overdrive_(false),
      timewarp_enabled_(true),
      timewarp_no_jit_enabled_(false),
      multisample_(true),
      position_tracking_enabled_(true),
      mipmap_eye_textures_(false),
      force_zero_ipd_(false),
      rendertarget_is_shared_by_both_eyes_(false),
      desired_pixel_density_(1.0f) {
  assert(this);
}

void Settings::Load() {
  assert(this);

  /// @todo move this file to userspace
  const std::string filename = "settings.js";
  std::ifstream in(filename.c_str());
  if (!in.good()) {
    return;
  }
  Json::Value root;
  Json::Reader reader;
  if (false == reader.parse(in, root)) {
    throw std::logic_error(Str() << "Unable to parse " << filename << ": "
                                 << reader.getFormattedErrorMessages());
  }

  blackout_ = root.get("blackout", blackout()).asBool();
  width_ = root.get("width", width()).asInt();
  height_ = root.get("height", height()).asInt();
  fullscreen_ = root.get("fullscreen", fullscreen()).asBool();
  control_scheme_ = root.get("controlscheme", control_scheme()).asString();
  primary_display_id_ = root.get("window", primary_display_id()).asInt();

  oculus_vr_detection_ =
      DetermineOculusVrValue(root.get("oculusvr", "auto").asString());

  anisotropic_ = root.get("anisotropic", anisotropic()).asFloat();

  support_joystick_ = root.get("joystick", support_joystick()).asBool();

  vsync_enabled_ = root.get("vsync_enabled", vsync_enabled()).asBool();
  is_low_persistence_ =
      root.get("is_low_persistence", is_low_persistence()).asBool();
  dynamic_prediction_ =
      root.get("dynamic_prediction", dynamic_prediction()).asBool();
  display_sleep_ = root.get("display_sleep", display_sleep()).asBool();
  mirror_to_window_ = root.get("mirror_to_window", mirror_to_window()).asBool();
  pixel_luminance_overdrive_ = root.get("pixel_luminance_overdrive",
                                        pixel_luminance_overdrive()).asBool();
  timewarp_enabled_ = root.get("timewarp_enabled", timewarp_enabled()).asBool();
  timewarp_no_jit_enabled_ =
      root.get("timewarp_no_jit_enabled", timewarp_no_jit_enabled()).asBool();
  multisample_ = root.get("multisample", multisample()).asBool();
  position_tracking_enabled_ = root.get("position_tracking_enabled",
                                        position_tracking_enabled()).asBool();
  mipmap_eye_textures_ =
      root.get("mipmap_eye_textures", mipmap_eye_textures()).asBool();
  force_zero_ipd_ = root.get("force_zero_ipd", force_zero_ipd()).asBool();
  rendertarget_is_shared_by_both_eyes_ =
      root.get("rendertarget_is_shared_by_both_eyes",
               rendertarget_is_shared_by_both_eyes()).asBool();

  desired_pixel_density_ =
      root.get("desired_pixel_density", desired_pixel_density()).asFloat();
}

const bool Settings::blackout() const {
  assert(this);
  return blackout_;
}

const int Settings::width() const {
  assert(this);
  return width_;
}

const int Settings::height() const {
  assert(this);
  return height_;
}

const bool Settings::fullscreen() const {
  assert(this);
  return fullscreen_;
}

const std::string Settings::control_scheme() const {
  assert(this);
  return control_scheme_;
}

int Settings::primary_display_id() const {
  assert(this);
  return primary_display_id_;
}

OculusVrDetection Settings::oculus_vr_detection() const {
  assert(this);
  return oculus_vr_detection_;
}

float Settings::anisotropic() const {
  assert(this);
  return anisotropic_;
};

bool Settings::support_joystick() const {
  assert(this);
  return support_joystick_;
}

bool Settings::vsync_enabled() const {
  assert(this);
  return vsync_enabled_;
}

bool Settings::is_low_persistence() const {
  assert(this);
  return is_low_persistence_;
}

bool Settings::dynamic_prediction() const {
  assert(this);
  return dynamic_prediction_;
}

bool Settings::display_sleep() const {
  assert(this);
  return display_sleep_;
}

bool Settings::mirror_to_window() const {
  assert(this);
  return mirror_to_window_;
}

bool Settings::pixel_luminance_overdrive() const {
  assert(this);
  return pixel_luminance_overdrive_;
}

bool Settings::timewarp_enabled() const {
  assert(this);
  return timewarp_enabled_;
}

bool Settings::timewarp_no_jit_enabled() const {
  assert(this);
  return timewarp_no_jit_enabled_;
}

bool Settings::multisample() const {
  assert(this);
  return multisample_;
}

bool Settings::position_tracking_enabled() const {
  assert(this);
  return position_tracking_enabled_;
}

bool Settings::mipmap_eye_textures() const {
  assert(this);
  return mipmap_eye_textures_;
}

bool Settings::force_zero_ipd() const {
  assert(this);
  return force_zero_ipd_;
}

bool Settings::rendertarget_is_shared_by_both_eyes() const {
  assert(this);
  return rendertarget_is_shared_by_both_eyes_;
}

float Settings::desired_pixel_density() const {
  assert(this);
  return desired_pixel_density_;
}
}  // namespace euphoria
