// Euphoria - Copyright (c) Gustav

#include "euphoria/input-bind.h"
#include <cassert>
#include "euphoria/input-action.h"
#include "euphoria/input-globaltoggle.h"

namespace input {

std::string ToString(const BindType& bt) {
  switch (bt) {
    case BindType::Axis:
      return "axis";
    case BindType::Range:
      return "range";
    default:
      return "<unknown BindType>";
  }
}

std::ostream& operator<<(std::ostream& ss, const BindType& bt) {
  ss << ToString(bt);
  return ss;
}

Bind::Bind(InputAction* action, BindType type)
    : action_(action),
      type_(type),
      toggle_(action ? action->toggle() : 0),
      value_(0.0f) {
  assert(this);
  assert(action_);

  // if the action is a global, we should have a toggle
  assert(action_->global() == (toggle_ != 0));

  if (toggle_) {
    assert(action->global());  // the action should be global
    toggle_->Add(this);
  }
}

Bind::~Bind() {
  assert(this);
  if (toggle_) {
    toggle_->Remove(this);
  }
}

float Bind::value() const {
  assert(this);
  return value_;
}

void Bind::set_value(float v) {
  assert(this);
  value_ = v;
}

BindType Bind::type() const {
  assert(this);
  return type_;
}

InputAction* Bind::action() {
  assert(this);
  assert(action_);
  return action_;
}

}  // namespace input
