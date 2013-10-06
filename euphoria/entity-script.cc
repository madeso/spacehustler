  // Euphoria - Copyright (c) Gustav
// Euphoria is awesome: 幸福感是真棒

#include "euphoria/entity-script.h"
#include <cassert>
#include <string>
#include <vector>
#include <memory>
#include "json/json.h"

#include "euphoria/entity.h"
#include "euphoria/lua.h"
#include "euphoria/script.h"

const std::string ScriptingSystemType = "Script";

class ScriptType : public ComponentType {
  public:
    ScriptType(const Json::Value& data, Lua* script)
      : table(script) {
    }

    Table table;
};

class ScriptObject {
  public:
    ScriptObject(Entity* entity, ScriptType* type, Lua* script)
      : entity(entity)
      , table(script) {
    }

    Entity* entity;
    Table table;
};

/// @todo add init function and 1 self table per object so variables can be
/// kept between updates
class ScriptSystem : public System {
  public:
    ScriptSystem(Json::Value data, Lua* ascript)
      : script_(ascript)
      , type_function_(data.get("typefunction", "").asString())
      , component_function_(data.get("componentfunction", "").asString())
      , step_function_(data.get("stepfunction", "").asString()) {
      assert(this);
    }

    ComponentType* AddType(const Json::Value& data) {
      assert(this);
      std::shared_ptr<ScriptType> type(new ScriptType(data, script_));
      types_.push_back(type);

      Json::Value temp = data;
      FunctionCall f(script_->state(), type_function_);
      f.Argument(type->table);
      f.Argument(&temp);
      f.Call();

      return type.get();
    }

    virtual void AddComponent(Entity* entity, ComponentType* type) {
      assert(this);
      assert(entity);
      assert(type);

      ScriptType* script_type = static_cast<ScriptType*>(type);

      std::shared_ptr<ScriptObject> o(new ScriptObject(entity, script_type,
                                      script_));

      FunctionCall f(script_->state(), component_function_);
      f.Argument(o->table);
      f.Argument(o->entity->table);
      f.Argument(script_type->table);
      f.Call();

      objects_.push_back(o);
    }

    void Step(float dt) {
      assert(this);

      for (auto & o : objects_) {
        if (step_function_.empty() == false) {
          FunctionCall f(script_->state(), step_function_);
          f.Argument(o->table);
          f.Argument(o->entity->table);
          f.Argument(o->entity);
          f.Argument(dt);
          f.Call();  /// @todo add more arguments
        }
      }
    }

  private:
    Lua* script_;
    const std::string type_function_;
    const std::string component_function_;
    const std::string step_function_;
    std::vector<std::shared_ptr<ScriptType>> types_;
    std::vector<std::shared_ptr<ScriptObject>> objects_;
};


void AddScriptCallback(const CreateSystemArg& arg, Json::Value data) {
  const std::string name(data.get("name", "").asString());
  std::shared_ptr<System> sys(new ScriptSystem(data, arg.script));
  arg.container->Add(name, sys);
}

void Entity_AddScript(SystemCreatorList* sc) {
  sc->Add(ScriptingSystemType, AddScriptCallback);
}

namespace scriptingjson {
  // -- Module: Json
  const std::string LUA_MODULE_NAME = "Json";

  // -- Function: Get
  // -- Description: Gets a value from the json data
  // -- Arguments:
  // -- Json the json data to get the value from
  // -- String the name of the variable
  // -- Number the default
  // -- Returns:
  // -- Number the value or the default
  void GetValue(ScriptParams* params) {
    assert(params);
    Json::Value* value = 0;
    std::string name = "";

    float f = 0.0f;

    if (ScriptOverload(params) << cLightUserData(&value) << &name << &f) {
      float ret = value->get(name, f).asFloat();
      params->Return(ret);
    }
  }
  REGISTER_SCRIPT_FUNCTION("Get", GetValue);
}  // namespace scriptingjson
