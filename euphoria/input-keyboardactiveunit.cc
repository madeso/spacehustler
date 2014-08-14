// Euphoria - Copyright (c) Gustav

#include "euphoria/input-keyboardactiveunit.h"
#include <cassert>

#include "euphoria/input-director.h"
#include "euphoria/input-action.h"
#include "euphoria/input-commondef.h"

namespace euphoria {

namespace input {

KeyboardActiveUnit::KeyboardActiveUnit(
    const std::vector<std::shared_ptr<TRangeBind<Key>>>& binds,
    InputDirector* director)
    : director_(director),
      actions_(ConvertToBindMap<TRangeBind<Key>, Key>(binds)) {
  assert(this);
  assert(director_);

  director_->Add(this);
}

void KeyboardActiveUnit::OnKey(const Key& key, bool state) {
  assert(this);
  auto actionsit = actions_.find(key);
  if (actionsit != actions_.end()) {
    TransformAndSetBindValue(actionsit->second, state ? 1.0f : 0.0f);
  }
}

KeyboardActiveUnit::~KeyboardActiveUnit() { director_->Remove(this); }

void KeyboardActiveUnit::Rumble() {}

}  // namespace input
}  // namespace euphoria
