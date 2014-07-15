// Euphoria - Copyright (c) Gustav

#include "euphoria/input-keyconfig.h"
#include <cassert>

#include "euphoria/input-unitdef.h"
#include "euphoria/input-bindmap.h"
#include "euphoria/input-activelist.h"
#include "euphoria/input-director.h"
namespace input {

KeyConfig::KeyConfig() { assert(this); }

void KeyConfig::Add(std::shared_ptr<UnitDef> def) {
  assert(this);
  assert(def);
  definitions_.push_back(def);
}

ConnectedUnits KeyConfig::Connect(const InputActionMap& actions,
                                  InputDirector* director) {
  assert(this);
  assert(director);
  std::shared_ptr<ActiveList> actives(new ActiveList());
  binds_.reset(new BindMap(actions, actives.get()));

  ConnectedUnits units(actives);
  for (auto def : definitions_) {
    auto unit = def->Create(director, binds_.get());
    assert(unit);
    units.Add(unit);
  }
  return units;
}

}  // namespace input
