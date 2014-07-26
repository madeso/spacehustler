// Euphoria - Copyright (c) Gustav

#include "euphoria/input-mouseactiveunit.h"
#include <cassert>

#include "euphoria/input-director.h"
#include "euphoria/input-action.h"

namespace input {

template <typename TBind, typename Type>
std::map<Type, std::shared_ptr<Bind>> ConvertToBindMap(
    ActiveUnit* au, const std::vector<std::shared_ptr<TBind>>& axis) {
  std::map<Type, std::shared_ptr<Bind>> actions_;
  for (auto a : axis) {
    au->Add(a->bind()->action());
    actions_.insert(std::make_pair(a->type(), a->bind()));
  }
  return actions_;
}

MouseActiveUnit::MouseActiveUnit(
    const std::vector<std::shared_ptr<TAxisBind<Axis::Type>>>& axis,
    const std::vector<std::shared_ptr<TRangeBind<MouseButton::Type>>>& buttons,
    InputDirector* director)
    : director_(director) {
  assert(this);
  assert(director_);

  actions_ = ConvertToBindMap<TAxisBind<Axis::Type>, Axis::Type>(this, axis);
  buttons_ = ConvertToBindMap<TRangeBind<MouseButton::Type>, MouseButton::Type>(
      this, buttons);

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
  assert(this);
  auto res = buttons_.find(key);
  if (res != buttons_.end()) {
    res->second->set_value(state);
  }
}

MouseActiveUnit::~MouseActiveUnit() { director_->Remove(this); }

void MouseActiveUnit::Rumble() {}

}  // namespace input
