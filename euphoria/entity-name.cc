// Euphoria - Copyright (c) Gustav

#include "euphoria/entity-camera.h"
#include <cassert>
#include <string>
#include <vector>
#include "euphoria/entity.h"
#include "euphoria/script.h"

const std::string NameSystemType = "Name";

struct NameType : public ComponentType {
 public:
  explicit NameType(const Json::Value& data)
      : name(data.get("name", "").asString()) {}

  const std::string name;
};

struct NameObject {
 public:
  NameObject(Entity* ent, const NameType& t) : entity(ent) { assert(entity); }

  Entity* entity;
};

class NameSystem;

namespace {
NameSystem*& Instance() {
  static NameSystem* instance = NULL;
  return instance;
}
}  // namespace

class NameSystem : public System {
 public:
  NameSystem() {
    assert(this);
    assert(Instance() == NULL);
    Instance() = this;
  }

  ComponentType* AddType(const Json::Value& data) {
    assert(this);
    std::shared_ptr<NameType> type(new NameType(data));
    types_.push_back(type);
    return type.get();
  }

  virtual void AddComponent(Entity* entity, ComponentType* type) {
    assert(this);
    assert(entity);
    assert(type);
    NameType* st = static_cast<NameType*>(type);

    entities_.insert(std::make_pair(st->name, entity));

    objects_.push_back(NameObject(entity, *st));
  }

  void Step(float dt) { assert(this); }

  Entity* GetEntity(const std::string& name) {
    auto r = entities_.find(name);
    if (r == entities_.end()) {
      return NULL;
    } else {
      return r->second;
    }
  }

 private:
  std::vector<std::shared_ptr<NameType> > types_;
  std::vector<NameObject> objects_;

  std::map<std::string, Entity*> entities_;
};

void AddNameCallback(const CreateSystemArg& arg, Json::Value data) {
  assert(arg.container);
  assert(arg.camera);
  std::shared_ptr<System> sys(new NameSystem());
  arg.container->Add(NameSystemType, sys);
}

void EntityAddName(SystemCreatorList* sc) {
  sc->Add(NameSystemType, AddNameCallback);
}

namespace name {
// -- Module: Names
const std::string LUA_MODULE_NAME = "Names";
// -- Function: FromName
void FromName(ScriptParams* params) {
  assert(params);

  std::string name;

  if (ScriptOverload(params) << &name) {
    if (Instance() == 0) {
      throw std::logic_error("Name system is not initialized.");
    }
    Entity* obj = Instance()->GetEntity(name);
    if (obj == NULL) {
      params->ReturnNil();
    } else {
      params->Return(obj);
    }
  }
}
REGISTER_SCRIPT_FUNCTION("FromName", FromName);
}  // namespace name
