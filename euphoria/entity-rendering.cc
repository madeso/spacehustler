// Euphoria - Copyright (c) Gustav

#include "euphoria/entity-rendering.h"
#include <cassert>
#include <string>
#include <vector>

#include "euphoria/entity.h"
#include "euphoria/world.h"
#include "euphoria/texturecache.h"
#include "euphoria/shadercache.h"

#include "euphoria/mesh.h"

const std::string RenderingSystemType = "Rendering";

class RenderType : public ComponentType {
  public:
    RenderType(const Json::Value& data, TextureCache* tc, ShaderCache* sc) {
      const std::string filename = data.get("file", "").asString();
      mesh.reset(new CompiledMesh(LoadMesh(filename) , tc, sc));
    }
    std::shared_ptr< CompiledMesh > mesh;
};

class RenderObject {
  public:
    RenderObject(Entity* entity, std::shared_ptr<Instance> instance)
      : entity(entity)
      , instance(instance) {
    }

    Entity* entity;
    std::shared_ptr<Instance> instance;
};

class RenderingSystem : public System {
  public:
    RenderingSystem(World* world, TextureCache* texture_cache,
                    ShaderCache* shader_cache)
      : world_(world), texture_cache_(texture_cache),
        shader_cache_(shader_cache) {
      assert(this);
      assert(world);
      assert(texture_cache);
      assert(shader_cache);
    }

    ComponentType* AddType(const Json::Value& data) {
      assert(this);
      std::shared_ptr<RenderType> type(new RenderType(data, texture_cache_,
                                       shader_cache_));
      types_.push_back(type);
      return type.get();
    }

    virtual void AddComponent(Entity* entity, ComponentType* type) {
      assert(this);
      assert(entity);
      assert(type);
      RenderType* st = static_cast<RenderType*>(type);
      mat44 mat = cmat44(entity->position, entity->rotation);
      std::shared_ptr<Instance> instance(new Instance(st->mesh, mat));
      world_->add(instance);
      objects_.push_back(RenderObject(entity, instance));
    }

    void Step(float dt) {
      assert(this);
      for (auto & o : objects_) {
        o.instance->transform = cmat44(o.entity->position,
                                       o.entity->rotation);
      }
    }

  private:
    World* world_;
    TextureCache* texture_cache_;
    ShaderCache* shader_cache_;

    std::vector<std::shared_ptr<RenderType> > types_;
    std::vector<RenderObject> objects_;
};


void AddRenderingCallback(CreateSystemArg arg, Json::Value data) {
  std::shared_ptr<System> sys(new RenderingSystem(arg.world, arg.texturecache,
                              arg.shadercache));
  arg.container->Add(RenderingSystemType, sys);
}

void Entity_AddRendering(SystemCreatorList* sc) {
  sc->add(RenderingSystemType, AddRenderingCallback);
}

