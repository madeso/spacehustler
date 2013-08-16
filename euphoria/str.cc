// Euphoria - Copyright (c) Gustav

#include "euphoria/str.h"

#include <cassert>
#include <string>

Str::operator std::string() const {
  assert(this);
  return toString();
}

std::string Str::toString() const {
  assert(this);
  return stream.str();
}
