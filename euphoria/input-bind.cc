// Euphoria - Copyright (c) Gustav

#include "euphoria/input-bind.h"
#include <cassert>

namespace input {

Bind::Bind(Action* action, BindType::Type type)
    : action_(action), type_(type), value_(0.0f) {
  assert(this);
  assert(action_);
}

float Bind::value() const {
  assert(this);
  return value_;
}

void Bind::set_value(float v) {
  assert(this);
  value_ = v;
}

BindType::Type Bind::type() const {
  assert(this);
  return type_;
}

Action* Bind::action() {
  assert(this);
  assert(action_);
  return action_;
}

}  // namespace input
