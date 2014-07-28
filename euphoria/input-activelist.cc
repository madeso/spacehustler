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

void ActiveList::Add(std::shared_ptr<ActiveRange> range) {
  assert(this);
  range_binds_.push_back(range);
}

void ActiveList::Add(std::shared_ptr<ActiveAxis> axis) {
  assert(this);
  axis_binds_.push_back(axis);
}

void ActiveList::Add(std::shared_ptr<ActiveAxisToRange> axis) {
  assert(this);
  axis_to_range_binds_.push_back(axis);
}

void ActiveList::Add(std::shared_ptr<ActiveRangeToAxis> axis) {
  assert(this);
  range_to_axis_binds_.push_back(axis);
}

void ActiveList::Add(std::shared_ptr<ActiveMasterAxis> axis) {
  assert(this);
  master_axis_binds_.push_back(axis);
}

void ActiveList::Add(std::shared_ptr<ActiveMasterRange> axis) {
  assert(this);
  master_range_binds_.push_back(axis);
}

void ActiveList::UpdateTable(Table* table) {
  assert(this);
  assert(table);

  for (auto b : master_range_binds_) {
    table->Set(b->action().scriptvarname(), b->state());
  }

  for (auto b : master_axis_binds_) {
    table->Set(b->action().scriptvarname(), b->state());
  }
}

void ActiveList::Update(float dt) {
  for (auto range : range_binds_) {
    range->Update(dt);
  }
  for (auto axis : axis_binds_) {
    axis->Update(dt);
  }
  for (auto axis : axis_to_range_binds_) {
    axis->Update(dt);
  }
  for (auto axis : range_to_axis_binds_) {
    axis->Update(dt);
  }
  for (auto axis : master_axis_binds_) {
    axis->Update(dt);
  }
  for (auto axis : master_range_binds_) {
    axis->Update(dt);
  }
}

}  // namespace input
