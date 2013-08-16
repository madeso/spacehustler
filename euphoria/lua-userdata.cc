// Euphoria - Copyright (c) Gustav

#include "euphoria/lua-userdata.h"
#include <cassert>
#include <stdexcept>
#include "euphoria/str.h"

void operator delete(void* pMem, lua_State* state, const char* metatableName) {
  // do nothing as lua is supposed to handle this.
}

void* operator new(size_t size, lua_State* state, const char* metatableName) {
  void* ptr = lua_newuserdata(state, size);
  luaL_getmetatable(state, metatableName);
  assert(lua_istable(state, -1));  // verify that the metatable is a table
  lua_setmetatable(state, -2);
  return ptr;
}

