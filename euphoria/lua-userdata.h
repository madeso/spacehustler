// Euphoria - Copyright (c) Gustav

/** @file
Functions for easily bind lua user data.
This is based on code from http://lua-users.org/wiki/DoItYourselfCppBinding
 */

#ifndef EUPHORIA_LUA_USERDATA_H_
#define EUPHORIA_LUA_USERDATA_H_

extern "C" {
#include "lua/lua.h"
#include "lua/lualib.h"
#include "lua/lauxlib.h"
}

/** Overloaded operator delete for ignoring compiler warnings.
Lua have allocated the memory so we won't delete it.
@param pMem the memory that wont be deleted.
@param state the lua state
@param metatableName the meta table name
 */
void operator delete(void* pMem, lua_State* state, const char* metatableName);

/** Overloaded new for creating userdata memory.
@param size how much memory to allocate
@param state the lua state
@param metatableName the meta table name
 */
void* operator new(size_t size, lua_State* state, const char* metatableName);

/** Creates a new object and calls placement new on it.
Assumes that the metatable is the same name as the class.
@param state the lua state
@param Class the class name.
 */
#define lua_pushobject(state, Class) new(state, #Class) Class

/** Generic lua gc function for use with lua_pushobject.
@param state the lua state.
@returns 0
 */
template<typename Class>
int GCMethod(lua_State* state) {
  reinterpret_cast<Class*>(lua_touserdata(state, 1))->~Class();
  return 0;
}

#endif  // EUPHORIA_LUA_USERDATA_H_
