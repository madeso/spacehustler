// Euphoria - Copyright (c) Gustav

#include "euphoria/input-connectedunits.h"
#include <cassert>
#include <string>

#include "euphoria/input-activeunit.h"
#include "euphoria/input-activelist.h"

#include "euphoria/str.h"

namespace euphoria {

namespace input {

ConnectedUnits::ConnectedUnits(std::shared_ptr<ActiveList> actives)
    : actives_(actives) {
  assert(this);
  assert(actives);
}

void ConnectedUnits::Add(std::shared_ptr<ActiveUnit> unit) {
  assert(this);
  assert(unit);
  units_.push_back(unit);
}

void ConnectedUnits::UpdateTable(Table* table) {
  assert(this);
  assert(table);
  assert(actives_);

  // not really relevant but this is great for error checking
  if (units_.empty()) {
    const std::string error =
        Str() << "No units connected for table update to be completed";
    throw error;
  }

  actives_->UpdateTable(table);
}

void ConnectedUnits::Update(float dt) {
  assert(this);
  assert(actives_);

  actives_->Update(dt);
}

bool ConnectedUnits::IsEmpty() const {
  assert(this);
  return units_.empty();
}

}  // namespace input
}  // namespace euphoria
