// Euphoria - Copyright (c) Gustav

#include "euphoria/input-activelist.h"
#include <cassert>

#include "euphoria/input-activeaxis.h"
#include "euphoria/input-activerange.h"
#include "euphoria/input-activerangetoaxis.h"
#include "euphoria/input-activeaxistorange.h"
#include "euphoria/input-activemasteraxis.h"
#include "euphoria/input-activemasterrange.h"

#include "euphoria/input-action.h"
#include "euphoria/lua.h"

namespace input {

void ActiveList::add(std::shared_ptr<ActiveRange> range) {
  assert(this);
  rangeBinds_.push_back(range);
}

void ActiveList::add(std::shared_ptr<ActiveAxis> axis) {
  assert(this);
  axisBinds_.push_back(axis);
}

void ActiveList::add(std::shared_ptr<ActiveAxisToRange> axis) {
  assert(this);
  axisToRangeBinds_.push_back(axis);
}

void ActiveList::add(std::shared_ptr<ActiveRangeToAxis> axis) {
  assert(this);
  rangeToAxisBinds_.push_back(axis);
}

void ActiveList::add(std::shared_ptr<ActiveMasterAxis> axis) {
  assert(this);
  masterAxisBinds_.push_back(axis);
}

void ActiveList::add(std::shared_ptr<ActiveMasterRange> axis) {
  assert(this);
  masterRangeBinds_.push_back(axis);
}

void ActiveList::UpdateTable(Table* table) {
  assert(this);
  assert(table);

  for (auto b : masterRangeBinds_) {
    table->Set(b->action().scriptvarname(), b->state());
  }

  for (auto b : masterAxisBinds_) {
    table->Set(b->action().scriptvarname(), b->state());
  }
}

void ActiveList::Update(float dt) {
  for (auto range : rangeBinds_) {
    range->update(dt);
  }
  for (auto axis : axisBinds_) {
    axis->update(dt);
  }
  for (auto axis : axisToRangeBinds_) {
    axis->update(dt);
  }
  for (auto axis : rangeToAxisBinds_) {
    axis->update(dt);
  }
  for (auto axis : masterAxisBinds_) {
    axis->update(dt);
  }
  for (auto axis : masterRangeBinds_) {
    axis->update(dt);
  }
}

}  // namespace input
