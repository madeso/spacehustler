// Euphoria - Copyright (c) Gustav

#include "euphoria/lua.h"

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

  FunctionReturn::~FunctionReturn() {
  }
}  // namespace internal

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
  : state(astate), args(0) {
  /// @todo save and push in call function so multiple calls can be made
  /// from a single function lookup
  lua_getglobal(state, name.c_str());
}

void FunctionCall::arg(const std::string& str) {
  lua_pushstring(state, str.c_str());
  ++args;
}

void FunctionCall::arg(int i) {
  lua_pushnumber(state, i);
  ++args;
}

void FunctionCall::ret(std::string* str) {
  std::shared_ptr<internal::FunctionReturn> r(new StringFunctionReturn(str));
  returns.push_back(r);
}

void FunctionCall::ret(int* i) {
  std::shared_ptr<internal::FunctionReturn> r(new IntFunctionReturn(i));
  returns.push_back(r);
}

void FunctionCall::call() {
  lua_call(state, args, returns.size());
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
}

Lua::~Lua() {
  assert(this);
  assert(state);
  lua_close(state);
}

void Lua::addStandardLibraries() {
  assert(this);
  assert(state);
  luaL_openlibs(state);
}

void Lua::runFile(const std::string& filename) {
  assert(this);
  assert(state);
  throwIfError(luaL_dofile(state, filename.c_str()));
}

void Lua::runCode(const std::string& code) {
  assert(this);
  assert(state);
  throwIfError(luaL_dostring(state, code.c_str()));
}

lua_State* Lua::getState() {
  assert(this);
  assert(state);
  return state;
}

void Lua::throwIfError(int errorcode) {
  assert(this);
  assert(state);
  if (errorcode) {
    throw std::logic_error(Str() << "Lua error: " << lua_tostring(state, -1));
  }
}
