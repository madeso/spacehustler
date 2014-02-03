// Euphoria - Copyright (c) Gustav

#include "euphoria/input-director.h"
#include <cassert>

#include "euphoria/input-keyboardactiveunit.h"
#include "euphoria/input-mouseactiveunit.h"

namespace input {

InputDirector::InputDirector() { assert(this); }

InputDirector::~InputDirector() { assert(this); }

void InputDirector::Add(KeyboardActiveUnit* kb) {
  assert(this);
  assert(kb);
  keyboards_.push_back(kb);
}

void InputDirector::Remove(KeyboardActiveUnit* kb) {
  assert(this);
  assert(kb);
  auto res = std::find(keyboards_.begin(), keyboards_.end(), kb);
  if (res != keyboards_.end()) {
    /// @todo implement as a swap back and erase function
    keyboards_.erase(res);
  }
}

void InputDirector::Add(MouseActiveUnit* au) {
  assert(this);
  assert(au);
  mouse_.push_back(au);
}

void InputDirector::Remove(MouseActiveUnit* au) {
  assert(this);
  assert(au);
  auto res = std::find(mouse_.begin(), mouse_.end(), au);
  if (res != mouse_.end()) {
    /// @todo implement as a swap back and erase function
    mouse_.erase(res);
  }
}

void InputDirector::OnKeyboardKey(Key::Type key, bool down) {
  assert(this);
  for (auto kb : keyboards_) {
    kb->OnKey(key, down);
  }
}

void InputDirector::OnMouseAxis(Axis::Type axis, float value) {
  assert(this);

  for (auto m : mouse_) {
    m->OnAxis(axis, value);
  }
}

void InputDirector::OnMouseButton(MouseButton::Type key, bool down) {
  assert(this);
}

void InputDirector::OnJoystickPov(Axis::Type type, int joystick, float value) {
  assert(this);
}

void InputDirector::OnJoystickButton(int button, int joystick, bool down) {
  assert(this);
}

void InputDirector::OnJoystickAxis(int axis, int joystick, float value) {
  assert(this);
}

}  // namespace input