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

OculusVrDetection::Type DetermineOculusVrValue(const std::string& name) {
  std::map<std::string, OculusVrDetection::Type> values;
  values.insert(std::make_pair("auto", OculusVrDetection::Auto));
  values.insert(std::make_pair("normal", OculusVrDetection::Normal));
  values.insert(std::make_pair("oculusvr", OculusVrDetection::Oculusvr));
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
      oculus_vr_detection_(OculusVrDetection::Auto),
      anisotropic_(1.0f),
      support_joystick_(true) {
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

OculusVrDetection::Type Settings::oculus_vr_detection() const {
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
