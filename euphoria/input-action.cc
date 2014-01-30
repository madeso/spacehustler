// Euphoria - Copyright (c) Gustav

#include "euphoria/input-action.h"
#include <cassert>
#include "euphoria/scalar.h"

namespace input {

InputAction::InputAction(const std::string& scriptvarname, Range::Type range)
    : scriptvarname_(scriptvarname), state_(0.0f), range_(range) {
  assert(this);
}

const std::string& InputAction::scriptvarname() const {
  assert(this);
  return scriptvarname_;
}

float InputAction::state() const {
  assert(this);
  return state_;
}

void InputAction::set_state(float state) {
  assert(this);
  float value = state;

  switch (range_) {
  case Range::Infinite:
    /* do nothing */
    break;
  case Range::Within01:
    value = KeepWithin(0.0f, value, 1.0f);
    break;
  case Range::WithinNegative1Positive1:
    value = KeepWithin(-1.0f, value, 1.0f);
    break;
  case Range::Invalid:
  default:
    assert(0 && "Using invalid value");
    break;
  }

  state_ = value;
}

}  // namespace input
