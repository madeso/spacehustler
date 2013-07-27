// Euphoria - Copyright (c) Gustav

#include "euphoria/lua.h"

#include <string>
#include <cassert>
#include <stdexcept>
#include "euphoria/str.h"
#include "euphoria/script.h"
#include "euphoria/scriptlib.h"

extern "C" {
#include "lua/lua.h"
#include "lua/lualib.h"
#include "lua/lauxlib.h"
}

namespace internal {
  FunctionReturn::~FunctionReturn() {
  }
}  // namespace internal

namespace {
  void ThrowIfError(lua_State* state, int errorcode) {
    assert(state);
    if (errorcode) {
      throw std::logic_error(Str() << "Lua error: " << lua_tostring(state, -1));
      // lua_pop(L, 1); ?
    }
  }
}  // namespace

class IntFunctionReturn : public internal::FunctionReturn {
  public:
    explicit IntFunctionReturn(int* i) : theint(i) {
      assert(theint);
      assert(this);
    }

    void get(lua_State* state) {
      assert(theint);
      assert(this);
      *theint = lua_tointeger(state, -1);
    }

    int* theint;
};

class StringFunctionReturn : public internal::FunctionReturn {
  public:
    explicit StringFunctionReturn(std::string* s) : thestring(s) {
      assert(thestring);
      assert(this);
    }

    void get(lua_State* state) {
      assert(thestring);
      assert(this);
      *thestring = lua_tostring(state, -1);
    }

    std::string* thestring;
};

FunctionCall::FunctionCall(lua_State* astate, const std::string& name)
  : valid(true), state(astate), args(0) {
  assert(this);
  assert(state);
  /// @todo save and push in call function so multiple calls can be made
  /// from a single function lookup
  lua_getglobal(state, name.c_str());
}

void FunctionCall::arg(const std::string& str) {
  assert(this);
  assert(state);
  assert(valid);
  lua_pushstring(state, str.c_str());
  ++args;
}

void FunctionCall::arg(float f) {
  assert(this);
  assert(state);
  assert(valid);
  lua_pushnumber(state, f);
  ++args;
}

void FunctionCall::arg(int i) {
  assert(this);
  assert(state);
  assert(valid);
  lua_pushnumber(state, i);
  ++args;
}

void FunctionCall::arg(void* v) {
  assert(this);
  assert(state);
  assert(v);
  assert(valid);
  lua_pushlightuserdata(state, v);
  ++args;
}

void FunctionCall::ret(std::string* str) {
  assert(this);
  assert(state);
  assert(str);
  assert(valid);
  std::shared_ptr<internal::FunctionReturn> r(new StringFunctionReturn(str));
  returns.push_back(r);
}

void FunctionCall::ret(int* i) {
  assert(this);
  assert(state);
  assert(i);
  assert(valid);
  std::shared_ptr<internal::FunctionReturn> r(new IntFunctionReturn(i));
  returns.push_back(r);
}

void FunctionCall::call() {
  assert(this);
  assert(state);
  assert(args >= 0);
  assert(valid);
  ThrowIfError(state, lua_pcall(state, args, returns.size(), 0));
  for (auto a : returns) {
    a->get(state);
    lua_pop(state, 1);
  }
  args = 0;
  returns.clear();
}

Lua::Lua() : state(luaL_newstate()) {
  assert(this);
  assert(state);

  luaL_openlibs(state);
  scriptlib_register(state);
  assert(GetGlobalScriptRegister());
  GetGlobalScriptRegister()->registerAll(state);
}

Lua::~Lua() {
  assert(this);
  assert(state);
  lua_close(state);
}

void Lua::runFile(const std::string& filename) {
  assert(this);
  assert(state);
  ThrowIfError(state, luaL_dofile(state, filename.c_str()));
}

void Lua::runCode(const std::string& code) {
  assert(this);
  assert(state);
  ThrowIfError(state, luaL_dostring(state, code.c_str()));
}

void Lua::setGlobal(const std::string& name, float value) {
  assert(this);
  assert(state);
  lua_pushnumber(state, value);
  lua_setglobal(state, name.c_str());
}

lua_State* Lua::getState() {
  assert(this);
  assert(state);
  return state;
}
