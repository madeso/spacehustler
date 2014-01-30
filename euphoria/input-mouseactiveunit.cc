// Euphoria - Copyright (c) Gustav

#include "euphoria/input-mouseactiveunit.h"
#include <cassert>
namespace input {

  MouseActiveUnit::MouseActiveUnit(const std::vector<AxisBind<Axis::Type>>& axis,
    InputDirector* director)
    : director_(director) {
    assert(this);
    assert(director_);

    for (auto b : axis) {
      Add(b.action());
      actions_.insert(std::make_pair(b.type(), b));
    }

    director_->Add(this);
  }

  void MouseActiveUnit::OnAxis(const Axis::Type& key, float state) {
    assert(this);
    auto res = actions_.find(key);
    if (res != actions_.end()) {
      /// @todo use axisdata to change state
      res->second.action()->set_state(res->second.Process(state));
    }
  }

  MouseActiveUnit::~MouseActiveUnit() { director_->Remove(this); }

  void MouseActiveUnit::Rumble() {}

}  // namespace input
