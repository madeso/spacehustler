// Euphoria - Copyright (c) Gustav

#include "euphoria/input-activeunit.h"
#include <cassert>

#include "euphoria/input-action.h"

namespace input {

ActiveUnit::~ActiveUnit() { assert(this); }

ActiveUnit::ActiveUnit() { assert(this); }

void ActiveUnit::Add(std::shared_ptr<InputAction> action) {
  assert(this);
  assert(action);
  actions_.push_back(action);
}

}  // namespace input
