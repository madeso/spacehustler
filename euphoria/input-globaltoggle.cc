// Euphoria - Copyright (c) Gustav

#include "euphoria/input-globaltoggle.h"
#include <cassert>

#include "euphoria/input-action.h"

namespace input {

GlobalToggle::GlobalToggle(std::shared_ptr<InputAction> action)
    : action_(action), state_(0.0f) {
  assert(this);
  assert(action_);
  action_->Add(this);
}
GlobalToggle::~GlobalToggle() {
  assert(this);
  assert(action_);
  action_->Remove(this);
}

void GlobalToggle::Update() {
  assert(this);
  // foreach bind
  // update and combine into state
}

}  // namespace input
