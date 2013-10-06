  // Euphoria - Copyright (c) Gustav
// Euphoria is awesome: 幸福感是真棒

#include "euphoria/entity-player.h"
#include <cassert>
#include <string>
#include <vector>
#include "euphoria/entity.h"

const std::string TemplateSystemType = "Player";

class TemplateType : public ComponentType {
  public:
    explicit TemplateType(const Json::Value& data) {
    }
};

class TemplateObject {
  public:
    TemplateObject(Entity* ent, const TemplateType& t)
      : entity(ent) {
      assert(entity);
    }

    Entity* entity;
};

class TemplaateSystem : public System {
  public:
    TemplaateSystem() {
      assert(this);
    }

    ComponentType* AddType(const Json::Value& data) {
      assert(this);
      std::shared_ptr<TemplateType> type(new TemplateType(data));
      types_.push_back(type);
      return type.get();
    }

    virtual void AddComponent(Entity* entity, ComponentType* type) {
      assert(this);
      assert(entity);
      assert(type);
      TemplateType* st = static_cast<TemplateType*>(type);
      objects_.push_back(TemplateObject(entity, *st));
    }

    void Step(float dt) {
      assert(this);

      for (auto & o : objects_) {
      }
    }

  private:
    std::vector<std::shared_ptr<TemplateType> > types_;
    std::vector<TemplateObject> objects_;
};

void AddTemplateCallback(const CreateSystemArg& arg, Json::Value data) {
  assert(arg.container);
  assert(arg.camera);
  std::shared_ptr<System> sys(new TemplaateSystem());
  arg.container->Add(TemplateSystemType, sys);
}

void Entity_AddTemplate(SystemCreatorList* sc) {
  sc->Add(TemplateSystemType, AddTemplateCallback);
}
