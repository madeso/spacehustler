// Euphoria - Copyright (c) Gustav

#include "euphoria/input-action.h"
#include <cassert>

namespace input {

InputAction::InputAction(const std::string& scriptvarname, Range::Type range)
    : scriptvarname_(scriptvarname), range_(range) {
  assert(this);
}

const std::string& InputAction::scriptvarname() const {
  assert(this);
  return scriptvarname_;
}

const Range::Type InputAction::range() const {
  assert(this);
  return range_;
}

}  // namespace input
