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
