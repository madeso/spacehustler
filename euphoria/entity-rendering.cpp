// Euphoria - Copyright (c) Gustav

#include "euphoria/entity-rendering.h"
#include <cassert>

const EnumValue RenderingSystemType = SystemType().toEnum("Rendering");

class RenderingSystem : public System {
    void step(float dt) {
    }
};

void Entity_AddRendering(SystemContainer* container) {
  SystemPtr sys(new RenderingSystem());
  container->add(RenderingSystemType, sys);
}
