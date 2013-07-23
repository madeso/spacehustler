// Euphoria - Copyright (c) Gustav

#include "euphoria/script.h"

#include <string>
#include <cassert>
#include <stdexcept>
#include "euphoria/str.h"

extern "C" {
#include "lua/lua.h"
#include "lua/lualib.h"
#include "lua/lauxlib.h"
}

namespace internal {
  ScriptArgument::~ScriptArgument() {
  }
}  // namespace internal

namespace {
  class FloatScriptArgument : public internal::ScriptArgument {
    public:
      explicit FloatScriptArgument(float* adata) : data(adata) {
      }
      virtual bool isValid(lua_State* state, int position) {
        return lua_isnumber(state, position) == 1;
      }
      virtual void get(lua_State* state, int position) {
        *data = static_cast<float>(lua_tonumber(state, position));
      }

      float* data;
  };

  class UserDataScriptArgument : public internal::ScriptArgument {
    public:
      explicit UserDataScriptArgument(void** adata) : data(adata) {
      }
      virtual bool isValid(lua_State* state, int position) {
        return lua_islightuserdata(state, position);
      }
      virtual void get(lua_State* state, int position) {
        *data = lua_touserdata(state, position);
      }

      void** data;
  };
}  // namespace

ScriptOverload::ScriptOverload() : valid(false) {
  assert(this);
}
void ScriptOverload::define(void** userdata) {
  assert(this);
  std::shared_ptr<internal::ScriptArgument> ol(
    new UserDataScriptArgument(userdata));
  arguments.push_back(ol);
}
void ScriptOverload::define(float* f) {
  assert(this);
  std::shared_ptr<internal::ScriptArgument> ol(new FloatScriptArgument(f));
  arguments.push_back(ol);
}
ScriptOverload::operator bool() {
  assert(this);
  return isValid();
}
bool ScriptOverload::isValid() {
  assert(this);
  return valid;
}
bool ScriptOverload::validate(int argcount, lua_State* state) {
  assert(this);
  if (argcount != arguments.size()) {
    return false;
  }

  for (int n = 0; n < argcount; ++n) {
    if (false == arguments[n]->isValid(state, n + 1)) {
      return false;
    }
  }

  for (int n = 0; n < argcount; ++n) {
    assert(arguments[n]->isValid(state, n + 1));
    arguments[n]->get(state, n + 1);
  }
  valid = true;
  return true;
}

ScriptParams::ScriptParams(lua_State* astate) : state(astate), retcount(0) {
  assert(this);
}
void ScriptParams::overload(ScriptOverload* overload) {
  assert(this);
  overloads.push_back(overload);
}
void ScriptParams::fill() {
  assert(this);
  const int argumentcount = lua_gettop(state);
  for (auto ov : overloads) {
    if (ov->validate(argumentcount, state) == false) {
      return;
    }
  }
}
void ScriptParams::returnvar(void* userdata) {
  assert(this);
  lua_pushlightuserdata(state, userdata);
  ++retcount;
}
int ScriptParams::getReturnCount() {
  assert(this);
  return retcount;
}

ScriptRegister::ScriptRegister() {
  assert(this);
}
void ScriptRegister::add(const std::string& name, lua_CFunction func) {
  assert(this);
  functions.insert(std::make_pair(name, func));
}
void ScriptRegister::registerAll(lua_State* state) {
  assert(this);
  for (auto f : functions) {
    lua_pushcfunction(state, f.second);
    lua_setglobal(state, f.first.c_str());
  }
}

ScriptRegister* GetGlobalScriptRegister() {
  static ScriptRegister reg;
  return &reg;
}
