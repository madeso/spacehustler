// Euphoria - Copyright (c) Gustav

#include "euphoria/input-activerangetoaxis.h"
#include <cassert>
#include <string>

#include "euphoria/input-bind.h"
#include "euphoria/str.h"
#include "euphoria/input-action.h"

namespace input {

ActiveRangeToAxis::ActiveRangeToAxis(InputAction* action, Bind* positive,
                                     Bind* negative)
    : InputActive(action), positive_(positive), negative_(negative) {
  assert(this);
  assert(positive_);
  assert(negative_);

  if (positive_->type() != BindType::Range) {
    const std::string error = Str() << "bound type for positive "
                                    << action->name() << " is not a range, is "
                                    << positive_->type();
    throw error;
  }
  if (negative_->type() != BindType::Range) {
    const std::string error = Str() << "bound type for negative "
                                    << action->name() << " is not a range, is "
                                    << negative_->type();
    throw error;
  }
}

void ActiveRangeToAxis::Update(float dt) {
  assert(this);
  assert(positive_);
  assert(negative_);
  set_state(positive_->value() - negative_->value());
}

}  // namespace input
