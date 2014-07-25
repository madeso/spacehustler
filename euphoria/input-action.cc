// Euphoria - Copyright (c) Gustav

#include "euphoria/input-action.h"
#include <cassert>

namespace input {

InputAction::InputAction(const std::string& name,
                         const std::string& scriptvarname, Range::Type range,
                         bool global)
    : name_(name),
      scriptvarname_(scriptvarname),
      range_(range),
      global_(global),
      toggle_(0) {
  assert(this);
}

InputAction::~InputAction() {
  assert(this);
  assert(toggle_ == 0);
}

const std::string& InputAction::scriptvarname() const {
  assert(this);
  return scriptvarname_;
}

const Range::Type InputAction::range() const {
  assert(this);
  return range_;
}

const std::string& InputAction::name() const {
  assert(this);
  return name_;
}

bool InputAction::global() const {
  assert(this);
  return global_;
}

GlobalToggle* InputAction::toggle() {
  assert(this);
  return toggle_;
}

void InputAction::Setup(GlobalToggle* toggle) {
  assert(this);
  assert(toggle);
  assert(toggle_ == 0);
  toggle_ = toggle;
}

void InputAction::ClearToggle(GlobalToggle* toggle) {
  assert(this);
  assert(toggle);
  assert(toggle == toggle_);
  toggle_ = 0;
}

}  // namespace input
