// Euphoria - Copyright (c) Gustav

#include "euphoria/script.h"

#include <string>
#include <cassert>
#include <stdexcept>
#include <vector>
#include "euphoria/str.h"
#include "euphoria/stringmerger.h"

extern "C" {
#include "lua/lua.h"
#include "lua/lualib.h"
#include "lua/lauxlib.h"
}

namespace internal {
  ScriptArgument::~ScriptArgument() {
  }

  int HandleLuaException(const std::string& name, lua_State* state) {
    /// @todo grab exception information
    const std::string error =
      Str() << "Lua C function failure inside " << name;
    lua_pushstring(state, error.c_str());
    return lua_error(state);
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

      std::string toString() const {
        return "float";
      }

      float* data;
  };

  class UserDataScriptArgument : public internal::ScriptArgument {
    public:
      explicit UserDataScriptArgument(void** adata) : data(adata) {
      }

      virtual bool isValid(lua_State* state, int position) {
        auto ret = lua_islightuserdata(state, position) == 1;
        return ret;
      }

      virtual void get(lua_State* state, int position) {
        *data = lua_touserdata(state, position);
      }

      std::string toString() const {
        return "light user data";
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

std::vector<std::string> ScriptOverload::getArgumentTypes() const {
  std::vector<std::string> ret;
  for (auto a : arguments) {
    ret.push_back(a->toString());
  }
  return ret;
}

ScriptParams::ScriptParams(lua_State* astate) : state(astate), retcount(0) {
  assert(this);
}

void ScriptParams::overload(ScriptOverload* overload) {
  assert(this);
  overloads.push_back(overload);
}

std::vector<std::string> GetArgumentList(lua_State* state, int argcount) {
  std::vector<std::string> ret;
  for (int i = 1; i <= argcount; ++i) {
    const auto type = lua_type(state, i);
    std::string luatypename = "";
    switch (type) {
      case LUA_TNONE:
        luatypename = "none";
        break;
      case LUA_TNIL:
        luatypename = "nil";
        break;
      case LUA_TBOOLEAN:
        luatypename = "boolean";
        break;
      case LUA_TLIGHTUSERDATA:
        luatypename = "light user data";
        break;
      case LUA_TNUMBER:
        luatypename = "number";
        break;
      case LUA_TSTRING:
        luatypename = "string";
        break;
      case LUA_TTABLE:
        luatypename = "table";
        break;
      case LUA_TFUNCTION:
        luatypename = "function";
        break;
      case LUA_TUSERDATA:
        luatypename = "full user data";
        break;
      case LUA_TTHREAD:
        luatypename = "thread";
        break;
    }
    ret.push_back(luatypename);
  }
  return ret;
}

void ScriptParams::fill() {
  assert(this);
  const int argumentcount = lua_gettop(state);
  for (auto ov : overloads) {
    if (ov->validate(argumentcount, state)) {
      return;
    }
  }

  std::vector<std::string> overloadstrings;
  for (auto ov : overloads) {
    overloadstrings.push_back(StringMerger::Array()
                              .generate(ov->getArgumentTypes()));
  }
  const std::string calledwith = StringMerger::Array()
                                 .generate(GetArgumentList(state,
                                     argumentcount));
  const std::string alloverloads = StringMerger::EnglishOr()
                                   .generate(overloadstrings);

  throw std::logic_error(Str() <<
                         "Unable to determine function overload, called with "
                         << calledwith << " but expected one of"
                         << alloverloads);
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
