// Euphoria - Copyright (c) Gustav

#include "euphoria/input-activerange.h"
#include <cassert>

#include "euphoria/input-bind.h"

namespace input {

ActiveRange::ActiveRange(InputAction* action, Bind* range)
    : InputActive(action), range_(range) {
  assert(this);
  assert(range_);

  if (range_->type() != BindType::Axis) {
    throw "bound type is not a axis";
  }
}

void ActiveRange::update(float dt) {
  assert(this);
  assert(range_);
  set_state(range_->value());
}

}  // namespace input
