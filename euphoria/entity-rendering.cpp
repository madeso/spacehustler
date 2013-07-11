// Euphoria - Copyright (c) Gustav

#include "euphoria/entity-rendering.h"
#include <cassert>
#include <string>
#include <vector>
#include "euphoria/mesh.h"

const EnumValue RenderingSystemType = SystemType().toEnum("Rendering");

class RenderType : public ComponentType {
  public:
    RenderType(const Json::Value& data, TextureCache* tc, ShaderCache* sc) {
      const std::string filename = data.get("file", "").asString();
      mesh.reset(new CompiledMesh(LoadMesh(filename) , tc, sc));
    }
    boost::shared_ptr< CompiledMesh > mesh;
};

class RenderingSystem : public System {
  public:
    RenderingSystem(World* world, TextureCache* tc, ShaderCache* sc)
      : world(world), tc(tc), sc(sc) {
    }

    ComponentType* addType(const Json::Value& data) {
      boost::shared_ptr<RenderType> type(new RenderType(data, tc, sc));
      types.push_back(type);
      return type.get();
    }

    void step(float dt) {
    }

  private:
    World* world;
    TextureCache* tc;
    ShaderCache* sc;

    std::vector<boost::shared_ptr<RenderType> > types;
};

void Entity_AddRendering(SystemContainer* container, World* world,
                         TextureCache* tc, ShaderCache* sc) {
  SystemPtr sys(new RenderingSystem(world, tc, sc));
  container->add(RenderingSystemType, sys);
}
