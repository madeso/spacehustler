// Euphoria - Copyright (c) Gustav

#include "euphoria/input-keyboardactiveunit.h"
#include <cassert>

#include "euphoria/input-director.h"
#include "euphoria/input-action.h"

namespace input {

KeyboardActiveUnit::KeyboardActiveUnit(
    const std::vector<std::shared_ptr<TRangeBind<Key::Type>>>& binds,
    InputDirector* director)
    : director_(director) {
  assert(this);
  assert(director_);

  for (auto b : binds) {
    actions_.insert(std::make_pair(b->type(), b->bind()));
  }

  director_->Add(this);
}

void KeyboardActiveUnit::OnKey(const Key::Type& key, bool state) {
  assert(this);
  auto actionsit = actions_.find(key);
  if (actionsit != actions_.end()) {
    actionsit->second->set_value(state ? 1.0f : 0.0f);
  }
}

KeyboardActiveUnit::~KeyboardActiveUnit() { director_->Remove(this); }

void KeyboardActiveUnit::Rumble() {}

}  // namespace input
