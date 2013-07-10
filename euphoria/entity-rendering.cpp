// Euphoria - Copyright (c) Gustav

#include "euphoria/entity-rendering.h"
#include <cassert>
#include "euphoria/mesh.h"

const EnumValue RenderingSystemType = SystemType().toEnum("Rendering");

class RenderType {
public:
  boost::shared_ptr< CompiledMesh > mesh;
};

class RenderingSystem : public System {
    IdGenerator generator;

    Id* addType(const Json::Value& data) {
      return 0;
    }

    void step(float dt) {
    }
};

void Entity_AddRendering(SystemContainer* container) {
  SystemPtr sys(new RenderingSystem());
  container->add(RenderingSystemType, sys);
}
