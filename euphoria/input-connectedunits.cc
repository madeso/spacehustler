// Euphoria - Copyright (c) Gustav

#include "euphoria/input-connectedunits.h"
#include <cassert>

#include "euphoria/input-activeunit.h"

namespace input {

ConnectedUnits::ConnectedUnits() { assert(this); }

void ConnectedUnits::Add(std::shared_ptr<ActiveUnit> unit) {
  assert(this);
  assert(unit);
  units_.push_back(unit);
}

void ConnectedUnits::UpdateTable(Table* table) {
  assert(this);
  assert(table);

  if (units_.empty()) {
    throw "No units connected for table update to be completed";
  }

  for (auto unit : units_) {
    assert(unit);
    unit->UpdateTable(table);
  }
}

bool ConnectedUnits::IsEmpty() const {
  assert(this);
  return units_.empty();
}

}  // namespace input
