  // Euphoria - Copyright (c) Gustav
// Euphoria is awesome: 幸福感是真棒

#include "euphoria/settings.h"

#include <cassert>

Settings::Settings()
  : blackout_(false)
  , width_(800)
  , height_(600)
  , fullscreen_(false)
  , controlScheme_("keyboard") {
  assert(this);
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

const std::string Settings::constrolScheme() const {
  assert(this);
  return controlScheme_;
}
