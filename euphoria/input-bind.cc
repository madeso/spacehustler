// Euphoria - Copyright (c) Gustav

#include "euphoria/input-bind.h"
#include <cassert>

namespace input {

Bind::Bind(Action* action) : action_(action), value_(0.0f) {
  assert(this);
  assert(action);
}

float Bind::value() const {
  assert(this);
  return value_;
}

void Bind::set_value(float v) {
  assert(this);
  value_ = v;
}

}  // namespace input
