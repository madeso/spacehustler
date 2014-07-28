// Euphoria - Copyright (c) Gustav

#include "euphoria/input-mouseactiveunit.h"
#include <cassert>

#include "euphoria/input-director.h"
#include "euphoria/input-action.h"
#include "euphoria/input-commondef.h"

namespace input {

MouseActiveUnit::MouseActiveUnit(
    const std::vector<std::shared_ptr<TAxisBind<Axis>>>& axis,
    const std::vector<std::shared_ptr<TRangeBind<MouseButton::Type>>>& buttons,
    InputDirector* director)
    : director_(director),
      actions_(ConvertToBindMap<TAxisBind<Axis>, Axis>(axis)),
      buttons_(
          ConvertToBindMap<TRangeBind<MouseButton::Type>, MouseButton::Type>(
              buttons)) {
  assert(this);
  assert(director_);

  director_->Add(this);
}

void MouseActiveUnit::OnAxis(const Axis& key, float state) {
  assert(this);
  auto res = actions_.find(key);
  if (res != actions_.end()) {
    res->second->set_value(state);
  }
}

void MouseActiveUnit::OnButton(MouseButton::Type key, float state) {
  assert(this);
  auto res = buttons_.find(key);
  if (res != buttons_.end()) {
    res->second->set_value(state);
  }
}

MouseActiveUnit::~MouseActiveUnit() { director_->Remove(this); }

void MouseActiveUnit::Rumble() {}

}  // namespace input
