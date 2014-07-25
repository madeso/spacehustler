// Euphoria - Copyright (c) Gustav

#include "euphoria/input-activeaxis.h"
#include <cassert>
#include <string>

#include "euphoria/input-bind.h"

#include "euphoria/str.h"

namespace input {

ActiveAxis::ActiveAxis(InputAction* action, Bind* axis)
    : InputActive(action), axis_(axis) {
  assert(this);
  assert(axis_);

  if (axis_->type() != BindType::Axis) {
    const std::string error = Str() << "bound type is not a axis";
    throw error;
  }
}

void ActiveAxis::update(float dt) {
  assert(this);
  assert(axis_);
  set_state(axis_->value());
}

}  // namespace input
