// Euphoria - Copyright (c) Gustav

#include "euphoria/entity.h"
#include <cassert>

// no implementation for the Entity.

System::System() {
  assert(this);
}

System::~System() {
  assert(this);
}

SystemContainer::SystemContainer() {
  assert(this);
}

SystemContainer::~SystemContainer() {
  assert(this);
}

EnumType& SystemType() {
  static EnumType r;
  return r;
}

void SystemContainer::step(float dt) {
  assert(this);
  for (auto s : systems) {
    s.second->step(dt);
  }
}

void SystemContainer::add(EnumValue systemType, SystemPtr sys) {
  assert(this);
  systems.insert(SystemMap::value_type(systemType, sys));
}
