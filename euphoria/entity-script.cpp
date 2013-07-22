// Euphoria - Copyright (c) Gustav

#include "euphoria/entity-script.h"
#include <cassert>
#include <string>
#include <vector>
#include <memory>
#include "json/json.h"

#include "euphoria/entity.h"
#include "euphoria/lua.h"

const std::string ScriptingSystemType = "Rendering";

class ScriptType : public ComponentType {
  public:
    explicit ScriptType(const Json::Value& data) {
    }
};

class ScriptObject {
  public:
    ScriptObject(Entity* entity, ScriptType* type)
      : entity(entity) {
    }

    Entity* entity;
};

class ScriptSystem : public System {
  public:
    explicit ScriptSystem(Json::Value data)
      : functionname(data.get("function", "").asString()) {
      assert(this);
      const std::string filename = data.get("file", "").asString();
      script.addStandardLibraries();
      script.runFile(filename);
    }

    ComponentType* addType(const Json::Value& data) {
      assert(this);
      std::shared_ptr<ScriptType> type(new ScriptType(data));
      types.push_back(type);
      return type.get();
    }

    virtual void addComponent(Entity* entity, ComponentType* type) {
      assert(this);
      assert(entity);
      assert(type);
      objects.push_back(ScriptObject(entity, static_cast<ScriptType*>(type)));
    }

    void step(float dt) {
      assert(this);

      FunctionCall f(script.getState(), functionname);
      for (auto & o : objects) {
        f.call();  /// @todo add more arguments
      }
    }

  private:
    Lua script;
    const std::string functionname;
    std::vector<std::shared_ptr<ScriptType> > types;
    std::vector<ScriptObject> objects;
};


void AddScriptCallback(CreateSystemArg arg, Json::Value data) {
  const std::string name(data.get("name", "").asString());
  std::shared_ptr<System> sys(new ScriptSystem(data));
  arg.container->add(name, sys);
}

void Entity_AddScript(SystemCreatorList* sc) {
  sc->add(ScriptingSystemType, AddScriptCallback);
}

