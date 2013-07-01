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

void SystemContainer::step(float dt) {
  assert(this);
  for (auto s : systems) {
    s->step(dt);
  }
}

void SystemContainer::add(SystemPtr sys) {
  assert(this);
  systems.push_back(sys);
}
