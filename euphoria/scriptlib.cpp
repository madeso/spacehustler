// Euphoria - Copyright (c) Gustav

#include "euphoria/scriptlib.h"
#include <cassert>
#include <stdexcept>
#include "euphoria/str.h"
#include "euphoria/lua-userdata.h"
#include "euphoria/math.h"

namespace {
  int cvec3_create(lua_State* state) {
    vec3* v = lua_pushobject(state, vec3)(0, 0, 0);
    return 1;
  }

  static const luaL_Reg vec3lib[] = {
    {"create", cvec3_create}
    , {NULL, NULL}
  };

  static const luaL_Reg fvec[] = {
    {"__gc", GCMethod<vec3> }
    , {NULL, NULL}
  };

  void createmeta(lua_State* state) {
    luaL_newmetatable(state, "vec3");
    lua_pushvalue(state, -1);
    lua_setfield(state, -2, "__index");
    luaL_setfuncs(state, fvec, 0);
    lua_pop(state, 1);
  }

  LUAMOD_API int scriptlib_vec3_open(lua_State* state) {
    luaL_newlib(state, vec3lib);
    createmeta(state);
    return 1;
  }
}  // namespace

void scriptlib_register(lua_State* state) {
  luaL_requiref(state, "cvec3", scriptlib_vec3_open, 1);
  lua_pop(state, 1);
}
