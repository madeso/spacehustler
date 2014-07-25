// Euphoria - Copyright (c) Gustav

#include "euphoria/input-mouseactiveunit.h"
#include <cassert>

#include "euphoria/input-director.h"
#include "euphoria/input-action.h"

namespace input {

MouseActiveUnit::MouseActiveUnit(
    const std::vector<std::shared_ptr<TAxisBind<Axis::Type>>>& axis,
    InputDirector* director)
    : director_(director) {
  assert(this);
  assert(director_);

  for (auto a : axis) {
    Add(a->bind()->action());
    actions_.insert(std::make_pair(a->axis(), a->bind()));
  }

  director_->Add(this);
}

void MouseActiveUnit::OnAxis(const Axis::Type& key, float state) {
  assert(this);
  auto res = actions_.find(key);
  if (res != actions_.end()) {
    res->second->set_value(state);
  }
}

void MouseActiveUnit::OnButton(MouseButton::Type key, float state) {
  // todo: implement me
}

MouseActiveUnit::~MouseActiveUnit() { director_->Remove(this); }

void MouseActiveUnit::Rumble() {}

}  // namespace input
