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

/** Check the valid argument against a class.
Assumes that the meta table name is the same as the classname.
@param state the lua state
@param pos the argument position
@param Class the classname to cast to.
 */
#define lua_userdata_cast(state, pos, Class) reinterpret_cast<Class*>\
  (luaL_checkudata((state), (pos), #Class))

/** Overloaded new for creating userdata memory.
@param size how much memory to allocate
@param state the lua state
@param metatableName the meta table name
@returns the allocated object
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
