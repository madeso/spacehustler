// Euphoria - Copyright (c) Gustav

#include "euphoria/input-joystickactiveunit.h"
#include <cassert>

#include "euphoria/input-director.h"
#include "euphoria/input-action.h"
#include "euphoria/input-commondef.h"

namespace input {

JoystickActiveUnit::JoystickActiveUnit(
    int joystick, InputDirector* director,
    const std::vector<std::shared_ptr<TAxisBind<int>>>& axis,
    const std::vector<std::shared_ptr<TRangeBind<int>>>& buttons,
    const std::vector<std::shared_ptr<TAxisBind<HatAxis>>>& hats)
    : joystick_(joystick),
      director_(director),
      axis_(ConvertToBindMap<TAxisBind<int>, int>(axis)),
      buttons_(ConvertToBindMap<TRangeBind<int>, int>(buttons)),
      hats_(ConvertToBindMap<TAxisBind<HatAxis>, HatAxis>(hats)) {
  assert(this);
  assert(director_);

  director_->Add(this);
}

void JoystickActiveUnit::OnAxis(int axis, float state) {
  assert(this);
  auto actionsit = axis_.find(axis);
  if (actionsit != axis_.end()) {
    BindData data = actionsit->second;
    TransformAndSetBindValue(data, state);
  }
}

void JoystickActiveUnit::OnButton(int button, float state) {
  assert(this);
  auto actionsit = buttons_.find(button);
  if (actionsit != buttons_.end()) {
    TransformAndSetBindValue(actionsit->second, state);
  }
}

void JoystickActiveUnit::OnHat(const HatAxis& hatAxis, float state) {
  assert(this);
  auto actionsit = hats_.find(hatAxis);
  if (actionsit != hats_.end()) {
    TransformAndSetBindValue(actionsit->second, state);
  }
}

JoystickActiveUnit::~JoystickActiveUnit() { director_->Remove(this); }

void JoystickActiveUnit::Rumble() {}

}  // namespace input
