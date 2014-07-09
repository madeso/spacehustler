// Euphoria - Copyright (c) Gustav

#include "euphoria/input-activelist.h"
#include <cassert>

namespace input {

void ActiveList::add(std::shared_ptr<ActiveRange> range) {
  assert(this);
  rangeBinds_.push_back(range);
}

void ActiveList::add(std::shared_ptr<ActiveAxis> axis) {
  assert(this);
  axisBinds_.push_back(axis);
}

void ActiveList::add(std::shared_ptr<ActiveRangeToAxis> axis) {
  assert(this);
  rangeToAxisBinds_.push_back(axis);
}

void ActiveList::UpdateTable(Table* table) {
  assert(this);
  assert(table);
}

void ActiveList::update(float dt) {
  for (auto range : rangeBinds_) {
    range->update(dt);
  }
  for (auto axis : axisBinds_) {
    axis->update(dt);
  }
  for (auto axis : rangeToAxisBinds_) {
    axis->update(dt);
  }
}

}  // namespace input