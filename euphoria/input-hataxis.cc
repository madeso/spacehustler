// Euphoria - Copyright (c) Gustav

#include "euphoria/input-hataxis.h"
#include <cassert>

namespace input {

  HatAxis::HatAxis(int hat, Axis::Type axis) : hat_(hat), axis_(axis) {
    assert(this);
  }

  Axis::Type HatAxis::axis() const {
    assert(this);
    return axis_;
  }

  int HatAxis::hat() const {
    assert(this);
    return hat_;
  }

}  // namespace input
