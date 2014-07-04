// Euphoria - Copyright (c) Gustav

#include "euphoria/input-bindchildlist.h"
#include <cassert>

namespace input {

void BindChildList::add(std::shared_ptr<RangeBind> range) {
  assert(this);
  rangeBinds_.push_back(range);
}

void BindChildList::add(std::shared_ptr<AxisBind> axis) {
  assert(this);
  axisBinds_.push_back(axis);
}

void BindChildList::update(float dt) {
  for (auto range : rangeBinds_) {
    range->update(dt);
  }
  for (auto axis : axisBinds_) {
    axis->update(dt);
  }
}

}  // namespace input
