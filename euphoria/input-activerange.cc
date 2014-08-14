// Euphoria - Copyright (c) Gustav

#include "euphoria/input-activerange.h"
#include <cassert>
#include <string>

#include "euphoria/input-bind.h"
#include "euphoria/str.h"
#include "euphoria/input-action.h"

namespace euphoria {

namespace input {

ActiveRange::ActiveRange(InputAction* action, Bind* range)
    : InputActive(action), range_(range) {
  assert(this);
  assert(range_);

  if (range_->type() != BindType::RANGE) {
    const std::string error = Str() << "bound type for " << action->name()
                                    << " is not a range, is " << range_->type();
    throw error;
  }
}

void ActiveRange::Update(float dt) {
  assert(this);
  assert(range_);
  set_state(range_->value());
}

}  // namespace input
}  // namespace euphoria
