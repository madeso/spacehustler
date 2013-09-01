  // Euphoria - Copyright (c) Gustav
// Euphoria is awesome: 幸福感是真棒

#include "euphoria/str.h"

#include <cassert>
#include <string>

Str::operator std::string() const {
  assert(this);
  return ToString();
}

std::string Str::ToString() const {
  assert(this);
  return stream_.str();
}
