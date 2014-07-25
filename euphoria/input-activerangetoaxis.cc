// Euphoria - Copyright (c) Gustav

#include "euphoria/input-activerangetoaxis.h"
#include <cassert>
#include <string>

#include "euphoria/input-bind.h"
#include "euphoria/str.h"

namespace input {

ActiveRangeToAxis::ActiveRangeToAxis(InputAction* action, Bind* positive,
                                     Bind* negative)
    : InputActive(action), positive_(positive), negative_(negative) {
  assert(this);
  assert(positive_);
  assert(negative_);

  if (positive_->type() != BindType::Range) {
    const std::string error = Str() << "bound type is not a range";
    throw error;
  }
  if (negative_->type() != BindType::Range) {
    const std::string error = Str() << "bound type is not a range";
    throw error;
  }
}

void ActiveRangeToAxis::update(float dt) {
  assert(this);
  assert(positive_);
  assert(negative_);
  set_state(positive_->value() - negative_->value());
}

}  // namespace input
