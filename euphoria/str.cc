// Euphoria - Copyright (c) Gustav

#include "euphoria/str.h"

#include <cassert>
#include <string>

namespace euphoria {

Str::operator std::string() const {
  assert(this);
  return ToString();
}

std::string Str::ToString() const {
  assert(this);
  return stream_.str();
}
}  // namespace euphoria
