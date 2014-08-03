// Euphoria - Copyright (c) Gustav

#include "euphoria/input-mouseactiveunit.h"
#include <cassert>

#include "euphoria/input-director.h"
#include "euphoria/input-action.h"
#include "euphoria/input-commondef.h"

namespace input {

MouseActiveUnit::MouseActiveUnit(
    const std::vector<std::shared_ptr<TAxisBind<Axis>>>& axis,
    const std::vector<std::shared_ptr<TRangeBind<MouseButton>>>& buttons,
    InputDirector* director)
    : director_(director),
      actions_(ConvertToBindMap<TAxisBind<Axis>, Axis>(axis)),
      buttons_(
          ConvertToBindMap<TRangeBind<MouseButton>, MouseButton>(buttons)) {
  assert(this);
  assert(director_);

  director_->Add(this);
}

void MouseActiveUnit::OnAxis(const Axis& key, float state) {
  assert(this);
  auto res = actions_.find(key);
  if (res != actions_.end()) {
    TransformAndSetBindValue(res->second, state);
  }
}

void MouseActiveUnit::OnButton(MouseButton key, float state) {
  assert(this);
  auto res = buttons_.find(key);
  if (res != buttons_.end()) {
    TransformAndSetBindValue(res->second, state);
  }
}

MouseActiveUnit::~MouseActiveUnit() { director_->Remove(this); }

void MouseActiveUnit::Rumble() {}

}  // namespace input
