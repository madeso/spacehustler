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

  LightUserDataScriptArgument::LightUserDataScriptArgument(void** adata)
    : data(adata) {
    assert(this);
  }

  bool LightUserDataScriptArgument::isValid(lua_State* state, int position) {
    assert(this);
    auto ret = lua_islightuserdata(state, position) == 1;
    return ret;
  }

  void LightUserDataScriptArgument::get(lua_State* state, int position) {
    assert(this);
    *data = lua_touserdata(state, position);
  }

  std::string LightUserDataScriptArgument::toString() const {
    assert(this);
    return "light user data";
  }

  FullUserDataScriptArgument::FullUserDataScriptArgument(void** adata,
      const std::string& aname)
    : data(adata), name(aname) {
    assert(this);
  }

  bool FullUserDataScriptArgument::isValid(lua_State* state, int position) {
    assert(this);
    return luaL_checkudata(state, position, name.c_str()) != 0;
  }

  void FullUserDataScriptArgument::get(lua_State* state, int position) {
    assert(this);
    *data = luaL_checkudata(state, position, name.c_str());
  }

  std::string FullUserDataScriptArgument::toString() const {
    assert(this);
    return name;
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
}  // namespace

ScriptOverload::ScriptOverload(ScriptParams* aparams) : params(aparams)
  , valid(false) {
  assert(this);
  assert(params);
}

ScriptOverload& ScriptOverload::operator<<(
  std::shared_ptr < internal::ScriptArgument > arg) {
  assert(this);
  arguments.push_back(arg);
  return *this;
}

ScriptOverload& ScriptOverload::operator<<(float* f) {
  assert(this);
  std::shared_ptr<internal::ScriptArgument> ol(new FloatScriptArgument(f));
  arguments.push_back(ol);
  return *this;
}

ScriptOverload::operator bool() {
  assert(this);
  assert(params->isValidated() == false);

  bool val = validate(params->getArgumentCount(), params->getState());
  if (val) {
    params->setValidated();
  } else {
    std::vector<std::string> ret;
    for (auto a : arguments) {
      ret.push_back(a->toString());
    }
    params->addFailure(StringMerger::Array().generate(ret));
  }
  return val;
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

ScriptParams::ScriptParams(lua_State* astate) : state(astate), retcount(0)
  , argumentcount(0), validated(false) {
  assert(this);
  assert(astate);
  /// @todo move to the initializer list for speed and the ability make the
  /// variable a const
  argumentcount = lua_gettop(astate);
}

std::vector<std::string> GetArgumentList(lua_State* state, int argcount) {
  std::vector<std::string> ret;
  for (int i = 1; i <= argcount; ++i) {
    const auto type = lua_type(state, i);
    /// @todo replace with lua_typename
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

int ScriptParams::getArgumentCount() {
  assert(this);
  return argumentcount;
}

lua_State* ScriptParams::getState() {
  assert(this);
  assert(state);
  return state;
}

void ScriptParams::post() {
  assert(this);
  if (isValidated() == false) {
    const std::string calledwith = StringMerger::Array()
                                   .generate(GetArgumentList(state,
                                       argumentcount));
    const std::string alloverloads = StringMerger::EnglishOr()
                                     .generate(failures);

    throw std::logic_error(Str() <<
                           "Unable to determine function overload, called with "
                           << calledwith << " but expected one of"
                           << alloverloads);
  }
}

bool ScriptParams::isValidated() {
  assert(this);
  return validated;
}

void ScriptParams::setValidated() {
  assert(this);
  validated = true;
}

void ScriptParams::addFailure(const std::string& f) {
  assert(this);
  failures.push_back(f);
}

void ScriptParams::returnvar(void* userdata) {
  assert(this);
  assert(state);
  lua_pushlightuserdata(state, userdata);
  ++retcount;
}

lua_State* ScriptParams::returnFullUserData() {
  assert(this);
  assert(state);
  ++retcount;
  return state;
}

int ScriptParams::getReturnCount() {
  assert(this);
  return retcount;
}

ScriptRegister::ScriptRegister() {
  assert(this);
}

void ScriptRegister::add(const std::string& namespaceName,
                         const std::string& name, lua_CFunction func) {
  assert(this);
  functions[namespaceName].insert(std::make_pair(name, func));
}

void ScriptRegister::registerAll(lua_State* state) {
  assert(this);
  for (auto ns : functions) {
    lua_newtable(state);  /// @todo change to luaL_newlibtable / luaL_newlib
    for (auto f : ns.second) {
      lua_pushstring(state, f.first.c_str());
      lua_pushcfunction(state, f.second);
      lua_settable(state, -3);
    }
    lua_setglobal(state, ns.first.c_str());
  }
}

ScriptRegister* GetGlobalScriptRegister() {
  static ScriptRegister reg;
  return &reg;
}
