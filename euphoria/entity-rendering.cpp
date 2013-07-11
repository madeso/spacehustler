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

class RenderObject {
  public:
    RenderObject(Entity* entity, boost::shared_ptr<Instance> instance)
      : entity(entity)
      , instance(instance) {
    }

    Entity* entity;
    boost::shared_ptr<Instance> instance;
};

class RenderingSystem : public System {
  public:
    RenderingSystem(World* world, TextureCache* tc, ShaderCache* sc)
      : world(world), tc(tc), sc(sc) {
      assert(this);
      assert(world);
      assert(tc);
      assert(sc);
    }

    ComponentType* addType(const Json::Value& data) {
      assert(this);
      boost::shared_ptr<RenderType> type(new RenderType(data, tc, sc));
      types.push_back(type);
      return type.get();
    }

    virtual void addComponent(Entity* entity, ComponentType* type) {
      assert(this);
      assert(entity);
      assert(type);
      RenderType* st = static_cast<RenderType*>(type);
      mat44 mat = cmat44(entity->position, entity->rotation);
      boost::shared_ptr<Instance> instance(new Instance(st->mesh, mat));
      world->add(instance);
      objects.push_back(RenderObject(entity, instance));
    }

    void step(float dt) {
      assert(this);
      for (auto & o : objects) {
        o.instance->transform = cmat44(o.entity->position,
                                       o.entity->rotation);
      }
    }

  private:
    World* world;
    TextureCache* tc;
    ShaderCache* sc;

    std::vector<boost::shared_ptr<RenderType> > types;
    std::vector<RenderObject> objects;
};

void Entity_AddRendering(SystemContainer* container, World* world,
                         TextureCache* tc, ShaderCache* sc) {
  SystemPtr sys(new RenderingSystem(world, tc, sc));
  container->add(RenderingSystemType, sys);
}
