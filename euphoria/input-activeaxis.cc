// Euphoria - Copyright (c) Gustav

#include "euphoria/input-activeaxis.h"
#include <cassert>
#include <string>

#include "euphoria/input-bind.h"
#include "euphoria/input-action.h"

#include "euphoria/str.h"

namespace input {

ActiveAxis::ActiveAxis(InputAction* action, Bind* axis)
    : InputActive(action), axis_(axis) {
  assert(this);
  assert(axis_);

  if (axis_->type() != BindType::AXIS) {
    const std::string error = Str() << "bound type for " << action->name()
                                    << " is not a axis, is " << axis_->type();
    throw error;
  }
}

void ActiveAxis::Update(float dt) {
  assert(this);
  assert(axis_);
  set_state(axis_->value());
}

}  // namespace input
