// Euphoria - Copyright (c) Gustav

#include "euphoria/entity-rendering.h"
#include <cassert>
#include <string>
#include <vector>

#include "euphoria/entity.h"
#include "euphoria/graphics/world.h"
#include "euphoria/graphics/texturecache.h"
#include "euphoria/graphics/shadercache.h"

#include "euphoria/graphics/mesh.h"

namespace euphoria {

class Settings;

const std::string RenderingSystemType = "Rendering";

struct RenderType : public ComponentType {
 public:
  RenderType(const Json::Value& data, TextureCache* tc, ShaderCache* sc,
             const Settings& settings) {
    const std::string filename = data.get("file", "").asString();
    mesh.reset(new CompiledMesh(LoadMesh(filename), tc, sc, settings));
  }
  std::shared_ptr<CompiledMesh> mesh;
};

struct RenderObject {
  RenderObject(Entity* entity, std::shared_ptr<Instance> instance)
      : entity(entity), instance(instance) {}

  Entity* entity;
  std::shared_ptr<Instance> instance;
};

class RenderingSystem : public System, NonCopyable {
 public:
  RenderingSystem(const Settings& settings, World* world,
                  TextureCache* texture_cache, ShaderCache* shader_cache)
      : settings_(settings),
        world_(world),
        texture_cache_(texture_cache),
        shader_cache_(shader_cache) {
    assert(this);
    assert(world);
    assert(texture_cache);
    assert(shader_cache);
  }

  ComponentType* AddType(const Json::Value& data) override {
    assert(this);
    std::shared_ptr<RenderType> type(
        new RenderType(data, texture_cache_, shader_cache_, settings_));
    types_.push_back(type);
    return type.get();
  }

  void AddComponent(Entity* entity, ComponentType* type) override {
    assert(this);
    assert(entity);
    assert(type);
    RenderType* st = static_cast<RenderType*>(type);
    Mat44 mat = CreateMat44(entity->position, entity->rotation);
    std::shared_ptr<Instance> instance(new Instance(st->mesh, mat));
    world_->Add(instance);
    objects_.push_back(RenderObject(entity, instance));
  }

  void Step(float dt) override {
    assert(this);
    for (auto& o : objects_) {
      o.instance->set_transform(
          CreateMat44(o.entity->position, o.entity->rotation));
    }
  }

 private:
  const Settings& settings_;
  World* world_;
  TextureCache* texture_cache_;
  ShaderCache* shader_cache_;

  std::vector<std::shared_ptr<RenderType> > types_;
  std::vector<RenderObject> objects_;
};

void AddRenderingCallback(const CreateSystemArg& arg, Json::Value data) {
  std::shared_ptr<System> sys(new RenderingSystem(
      arg.settings, arg.world, arg.texturecache, arg.shadercache));
  arg.container->Add(RenderingSystemType, sys);
}

void EntityAddRendering(SystemCreatorList* sc) {
  sc->Add(RenderingSystemType, AddRenderingCallback);
}
}  // namespace euphoria
