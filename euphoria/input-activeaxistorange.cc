// Euphoria - Copyright (c) Gustav

#include "euphoria/input-activeaxistorange.h"
#include <cassert>
#include <string>

#include "euphoria/input-bind.h"
#include "euphoria/str.h"
#include "euphoria/input-action.h"

namespace input {

ActiveAxisToRange::ActiveAxisToRange(InputAction* action, Bind* axis)
    : InputActive(action), axis_(axis) {
  assert(this);
  assert(axis_);

  if (axis_->type() != BindType::Axis) {
    const std::string error = Str() << "bound type for axis " << action->name()
                                    << " is not a axis, is " << axis_->type();
    throw error;
  }
}

void ActiveAxisToRange::update(float dt) {
  assert(this);
  assert(axis_);
  float v = axis_->value();
  if (v < 0) {
    v = 0;
  }
  set_state(v);
}

}  // namespace input
