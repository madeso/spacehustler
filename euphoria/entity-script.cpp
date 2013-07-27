// Euphoria - Copyright (c) Gustav

#include "euphoria/entity-script.h"
#include <cassert>
#include <string>
#include <vector>
#include <memory>
#include "json/json.h"

#include "euphoria/entity.h"
#include "euphoria/lua.h"

const std::string ScriptingSystemType = "Script";

class ScriptType : public ComponentType {
  public:
    ScriptType(const Json::Value& data, Lua* script)
      : table(script->getState()) {
    }

    Table table;
};

class ScriptObject {
  public:
    ScriptObject(Entity* entity, ScriptType* type, Lua* script)
      : entity(entity)
      , table(script->getState()) {
    }

    Entity* entity;
    Table table;
};

/// @todo add init function and 1 self table per object so variables can be
/// kept between updates
class ScriptSystem : public System {
  public:
    ScriptSystem(Json::Value data, Lua* ascript)
      : script(ascript)
      , typefunction(data.get("typefunction", "").asString())
      , componentfunction(data.get("componentfunction", "").asString())
      , stepfunction(data.get("stepfunction", "").asString()) {
      assert(this);
    }

    ComponentType* addType(const Json::Value& data) {
      assert(this);
      std::shared_ptr<ScriptType> type(new ScriptType(data, script));
      types.push_back(type);

      Json::Value temp = data;
      FunctionCall f(script->getState(), typefunction);
      f.arg(type->table);
      f.arg(&temp);
      f.call();

      return type.get();
    }

    virtual void addComponent(Entity* entity, ComponentType* type) {
      assert(this);
      assert(entity);
      assert(type);

      ScriptType* stype = static_cast<ScriptType*>(type);

      std::shared_ptr<ScriptObject> o(new ScriptObject(entity, stype , script));

      FunctionCall f(script->getState(), componentfunction);
      f.arg(o->table);
      f.arg(stype->table);
      f.call();

      objects.push_back(o);
    }

    void step(float dt) {
      assert(this);

      for (auto & o : objects) {
        if (stepfunction.empty() == false) {
          FunctionCall f(script->getState(), stepfunction);
          f.arg(o->table);
          f.arg(o->entity);
          f.arg(dt);
          f.call();  /// @todo add more arguments
        }
      }
    }

  private:
    Lua* script;
    const std::string typefunction;
    const std::string componentfunction;
    const std::string stepfunction;
    std::vector<std::shared_ptr<ScriptType>> types;
    std::vector<std::shared_ptr<ScriptObject>> objects;
};


void AddScriptCallback(CreateSystemArg arg, Json::Value data) {
  const std::string name(data.get("name", "").asString());
  std::shared_ptr<System> sys(new ScriptSystem(data, arg.script));
  arg.container->add(name, sys);
}

void Entity_AddScript(SystemCreatorList* sc) {
  sc->add(ScriptingSystemType, AddScriptCallback);
}

