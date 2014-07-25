// Euphoria - Copyright (c) Gustav

#include "euphoria/input-globaltoggle.h"
#include <cassert>

#include "euphoria/input-action.h"
#include "euphoria/stdutils.h"
#include "euphoria/input-bind.h"

namespace input {

const float TRUE_VALUE = 0.8f;

GlobalToggle::GlobalToggle(std::shared_ptr<InputAction> action)
    : action_(action),
      state_(false),
      last_down_(false),
      current_state_holder_(0) {
  assert(this);
  assert(action_);
  action_->Setup(this);
}
GlobalToggle::~GlobalToggle() {
  assert(this);
  assert(action_);
  action_->ClearToggle(this);
}

void GlobalToggle::Update() {
  assert(this);
  bool down;

  Bind* theBind = 0;

  if (current_state_holder_) {
    down = current_state_holder_->value() < TRUE_VALUE;
    theBind = current_state_holder_;
  } else {
    down = false;
    for (Bind* bind : binds_) {
      if (bind->value() > TRUE_VALUE) {
        down = true;
        theBind = bind;
      }
    }
  }

  // todo: toggle when released, instead of when pressed!
  if (down && !last_down_) {
    // toggle!
    if (state_) {
      state_ = false;
      current_state_holder_ = 0;
    } else {
      state_ = true;
      current_state_holder_ = theBind;
    }
  }
  last_down_ = down;
}

void GlobalToggle::Add(Bind* bind) {
  assert(this);
  binds_.push_back(bind);
}

void GlobalToggle::Remove(Bind* bind) {
  assert(this);
  SwapBackAndEraseObject(bind, &binds_);
}

bool GlobalToggle::state() const {
  assert(this);
  return state_;
}

}  // namespace input
