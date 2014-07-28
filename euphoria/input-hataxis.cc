// Euphoria - Copyright (c) Gustav

#include "euphoria/input-hataxis.h"
#include <cassert>

namespace input {

HatAxis::HatAxis(int hat, Axis axis) : hat_(hat), axis_(axis) { assert(this); }

Axis HatAxis::axis() const {
  assert(this);
  return axis_;
}

int HatAxis::hat() const {
  assert(this);
  return hat_;
}

bool HatAxis::operator<(const HatAxis& rhs) const {
  if (axis_ == rhs.axis_) {
    return hat_ < rhs.hat_;
  } else {
    return axis_ < rhs.axis_;
  }
}

}  // namespace input
