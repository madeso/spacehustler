// Euphoria - Copyright (c) Gustav

#include "euphoria/scriptlib.h"
#include <cassert>
#include <stdexcept>
#include "euphoria/str.h"
#include "euphoria/lua-userdata.h"
#include "euphoria/math.h"
#include "euphoria/script.h"

namespace {
  // -- Module: cvec3
  // -- Function: create
  // -- Description: Creates a 3 dimensional vector
  // -- Arguments:
  // -- Number The X coordinate
  // -- Number The Y coordinate
  // -- Number The Z coordinate
  // -- Returns: vec3 a vector
  // -- Description: Creates a vector at origin
  // -- Returns: vec3 a vector placed at origin
  void cvec3_create(ScriptParams* p) {
    float x = 0;
    float y = 0;
    float z = 0;

    if (ScriptOverload(p) << &x << &y << &z) {
      lua_pushobject(p->returnFullUserData(), vec3)(x, y, z);
    } else if (ScriptOverload(p)) {
      lua_pushobject(p->returnFullUserData(), vec3)(0, 0, 0);
    }
  }
  SCRIPT_FUNCTION("cvec3.create", cvec3_create, lcvec3_create)

  static const luaL_Reg vec3lib[] = {
    {"create", lcvec3_create}
    , {NULL, NULL}
  };

  // -- Module: vec3
  // -- Function: add
  // -- Description: Add two vectors.
  // -- Arguments:
  // -- vec3 the left operand
  // -- vec3 the right operand
  // -- Returns: vec3 the sum of the 2 vectors.
  void vec3_add(ScriptParams* p) {
    vec3* a;
    vec3* b;
    if (ScriptOverload(p) << mFullUserData(vec3, &a)
        << mFullUserData(vec3, &b)) {
      assert(a);
      assert(b);
      vec3* r = lua_pushobject(p->returnFullUserData(), vec3)(0, 0, 0);
      *r = *a + *b;
    }
  }

  SCRIPT_FUNCTION("vec3.add", vec3_add, lvec3_add)

  static const luaL_Reg fvec[] = {
    {"__gc", GCMethod<vec3> }
    , {"__add", lvec3_add }
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
