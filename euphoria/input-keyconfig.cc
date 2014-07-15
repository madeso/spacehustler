// Euphoria - Copyright (c) Gustav

#include "euphoria/input-keyconfig.h"
#include <cassert>

#include "euphoria/input-unitdef.h"

namespace input {

KeyConfig::KeyConfig() { assert(this); }

void KeyConfig::Add(std::shared_ptr<UnitDef> def) {
  assert(this);
  assert(def);
  definitions_.push_back(def);
}

ConnectedUnits KeyConfig::Connect(InputDirector* director,
                                  BindMap* binds) const {
  assert(this);
  assert(director);
  assert(binds);

  ConnectedUnits units;
  for (auto def : definitions_) {
    auto unit = def->Create(director, binds);
    assert(unit);
    units.Add(unit);
  }
  return units;
}

}  // namespace input
