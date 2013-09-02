  // Euphoria - Copyright (c) Gustav
// Euphoria is awesome: 幸福感是真棒

#include "euphoria/settings.h"

#include <cassert>
#include <fstream> // NOLINT for loading data
#include <stdexcept>
#include "euphoria/str.h"
#include "json/json.h"

Settings::Settings()
  : blackout_(false)
  , width_(800)
  , height_(600)
  , fullscreen_(false)
  , control_scheme_("keyboard")
  , primary_display_id_(0) {
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
