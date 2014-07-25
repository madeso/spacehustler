// Euphoria - Copyright (c) Gustav

#include "euphoria/input-activerange.h"
#include <cassert>
#include <string>

#include "euphoria/input-bind.h"
#include "euphoria/str.h"

namespace input {

ActiveRange::ActiveRange(InputAction* action, Bind* range)
    : InputActive(action), range_(range) {
  assert(this);
  assert(range_);

  if (range_->type() != BindType::Axis) {
    const std::string error = Str() << "bound type is not a axis";
    throw error;
  }
}

void ActiveRange::update(float dt) {
  assert(this);
  assert(range_);
  set_state(range_->value());
}

}  // namespace input
