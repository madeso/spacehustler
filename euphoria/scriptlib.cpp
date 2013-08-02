// Euphoria - Copyright (c) Gustav

#include "euphoria/scriptlib.h"
#include <cassert>
#include <stdexcept>
#include <string>
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

  void cvec3_cardinal(ScriptParams* p) {
    int i = 0;

    if (ScriptOverload(p) << &i) {
      vec3* r = lua_pushobject(p->returnFullUserData(), vec3)(0, 0, 0);
      if (i > 3) {
        throw "Invalid cardinal index";
      }
      if (i <= 0) {
        throw "Invalid cardinal index";
      }
      r->cardinal(i - 1);
    }
  }
  SCRIPT_FUNCTION("cvec3.cardinal", cvec3_cardinal, lcvec3_cardinal)

  static const luaL_Reg vec3lib[] = {
    {"create", lcvec3_create}
    , {"cardinal", lcvec3_cardinal}
    , {NULL, NULL}
  };

  void vec3_index(ScriptParams* p) {
    vec3* a;
    std::string key;
    int index;
    if (ScriptOverload(p) << mFullUserData(vec3, &a) << &key) {
      assert(a);
      if (key == "x") {
        p->returnvar((*a)[0]);
      } else if (key == "y") {
        p->returnvar((*a)[1]);
      } else if (key == "z") {
        p->returnvar((*a)[2]);
      } else {
        throw "Invalid key";
      }
    } else if (ScriptOverload(p) << mFullUserData(vec3, &a) << &index) {
      assert(a);
      if (index <= 0) {
        throw "Invalid index";
      }
      if (index > 3) {
        throw "Invalid index";
      }
      p->returnvar((*a)[index - 1]);
    }
  }
  SCRIPT_FUNCTION("vec3.index", vec3_index, lvec3_index)

  void vec3_newindex(ScriptParams* p) {
    vec3* a;
    std::string key;
    int index;
    float value;
    if (ScriptOverload(p) << mFullUserData(vec3, &a) << &key << &value) {
      assert(a);
      if (key == "x") {
        (*a)[0] = value;
      } else if (key == "y") {
        (*a)[1] = value;
      } else if (key == "z") {
        (*a)[2] = value;
      } else {
        throw "Invalid key";
      }
    } else if (ScriptOverload(p) << mFullUserData(vec3, &a) << &index
               << &value) {
      assert(a);
      if (index <= 0) {
        throw "Invalid index";
      }
      if (index > 3) {
        throw "Invalid index";
      }
      (*a)[index - 1] = value;
    }
  }
  SCRIPT_FUNCTION("vec3.newindex", vec3_newindex, lvec3_newindex)

    void vec3_unm(ScriptParams* p) {
      vec3* a;
      if (ScriptOverload(p) << mFullUserData(vec3, &a)) {
        assert(a);
        vec3* r = lua_pushobject(p->returnFullUserData(), vec3)(0, 0, 0);
        *r = -*a;
      }
  }
  SCRIPT_FUNCTION("vec3.unm", vec3_unm, lvec3_unm)

  // -- Type: vec3
  // -- Function: add
  // -- Description: Add two vectors.
  // -- Arguments:
  // -- vec3 the left operand
  // -- vec3 the right operand
  // -- Returns: vec3 the sum of the 2 vectors.
  void vec3_add(ScriptParams* p) {
    vec3* a;
    vec3* b;
    float f;
    if (ScriptOverload(p) << mFullUserData(vec3, &a)
        << mFullUserData(vec3, &b)) {
      assert(a);
      assert(b);
      vec3* r = lua_pushobject(p->returnFullUserData(), vec3)(0, 0, 0);
      *r = *a + *b;
    } else if (ScriptOverload(p) << mFullUserData(vec3, &a)
               << &f) {
      assert(a);
      vec3* r = lua_pushobject(p->returnFullUserData(), vec3)(0, 0, 0);
      *r = *a + vec3(f, f, f);
    }
    if (ScriptOverload(p) << &f << mFullUserData(vec3, &a)) {
      assert(a);
      vec3* r = lua_pushobject(p->returnFullUserData(), vec3)(0, 0, 0);
      *r = vec3(f, f, f) + *a;
    }
  }
  SCRIPT_FUNCTION("vec3.add", vec3_add, lvec3_add)

  

  void vec3_sub(ScriptParams* p) {
    vec3* a;
    vec3* b;
    float f;
    if (ScriptOverload(p) << mFullUserData(vec3, &a)
        << mFullUserData(vec3, &b)) {
      assert(a);
      assert(b);
      vec3* r = lua_pushobject(p->returnFullUserData(), vec3)(0, 0, 0);
      *r = *a - *b;
    } else if (ScriptOverload(p) << mFullUserData(vec3, &a)
               << &f) {
      assert(a);
      vec3* r = lua_pushobject(p->returnFullUserData(), vec3)(0, 0, 0);
      *r = *a - vec3(f, f, f);
    }
    if (ScriptOverload(p) << &f << mFullUserData(vec3, &a)) {
      assert(a);
      vec3* r = lua_pushobject(p->returnFullUserData(), vec3)(0, 0, 0);
      *r = vec3(f, f, f) - *a;
    }
  }
  SCRIPT_FUNCTION("vec3.sub", vec3_sub, lvec3_sub)

  void vec3_mul(ScriptParams* p) {
    vec3* a;
    float f;
    if (ScriptOverload(p) << mFullUserData(vec3, &a)
        << &f) {
      assert(a);
      vec3* r = lua_pushobject(p->returnFullUserData(), vec3)(0, 0, 0);
      *r = *a * f;
    }
    if (ScriptOverload(p) << &f << mFullUserData(vec3, &a)) {
      assert(a);
      vec3* r = lua_pushobject(p->returnFullUserData(), vec3)(0, 0, 0);
      *r = f** a;
    }
  }
  SCRIPT_FUNCTION("vec3.mul", vec3_mul, lvec3_mul)

  void vec3_div(ScriptParams* p) {
    vec3* a;
    float f;
    if (ScriptOverload(p) << mFullUserData(vec3, &a)
        << &f) {
      assert(a);
      vec3* r = lua_pushobject(p->returnFullUserData(), vec3)(0, 0, 0);
      *r = *a / f;
    }
  }
  SCRIPT_FUNCTION("vec3.div", vec3_div, lvec3_div)

  void vec3_length(ScriptParams* p) {
    vec3* a;
    if (ScriptOverload(p) << mFullUserData(vec3, &a)) {
      assert(a);
      p->returnvar(a->length());
    }
  }
  SCRIPT_FUNCTION("vec3.length", vec3_length, lvec3_length)

  void vec3_length2(ScriptParams* p) {
    vec3* a;
    if (ScriptOverload(p) << mFullUserData(vec3, &a)) {
      assert(a);
      p->returnvar(a->length_squared());
    }
  }
  SCRIPT_FUNCTION("vec3.length2", vec3_length2, lvec3_length2)

  void vec3_zero(ScriptParams* p) {
    vec3* a;
    if (ScriptOverload(p) << mFullUserData(vec3, &a)) {
      assert(a);
      a->zero();
    }
  }
  SCRIPT_FUNCTION("vec3.zero", vec3_zero, lvec3_zero)

  void vec3_set(ScriptParams* p) {
    vec3* a;
    float x;
    float y;
    float z;
    float all;
    if (ScriptOverload(p) << mFullUserData(vec3, &a) << &x << &y << &z) {
      assert(a);
      a->set(x, y, z);
    } else if (ScriptOverload(p) << mFullUserData(vec3, &a) << &all) {
      assert(a);
      a->set(all, all, all);
    }
  }
  SCRIPT_FUNCTION("vec3.set", vec3_set, lvec3_set)

  void vec3_maximize(ScriptParams* p) {
    vec3* a;
    float x;
    float y;
    float z;
    float all;
    vec3* o;
    if (ScriptOverload(p) << mFullUserData(vec3, &a) << &x << &y << &z) {
      assert(a);
      a->maximize(vec3(x, y, z));
    } else if (ScriptOverload(p) << mFullUserData(vec3, &a) << &all) {
      assert(a);
      a->maximize(vec3(all, all, all));
    } else if (ScriptOverload(p) << mFullUserData(vec3, &a)
               << mFullUserData(vec3, &o)) {
      assert(a);
      a->maximize(*o);
    }
  }
  SCRIPT_FUNCTION("vec3.maximize", vec3_maximize, lvec3_maximize)

  void vec3_minimize(ScriptParams* p) {
    vec3* a;
    float x;
    float y;
    float z;
    float all;
    vec3* o;
    if (ScriptOverload(p) << mFullUserData(vec3, &a) << &x << &y << &z) {
      assert(a);
      a->minimize(vec3(x, y, z));
    } else if (ScriptOverload(p) << mFullUserData(vec3, &a) << &all) {
      assert(a);
      a->minimize(vec3(all, all, all));
    } else if (ScriptOverload(p) << mFullUserData(vec3, &a)
               << mFullUserData(vec3, &o)) {
      assert(a);
      a->minimize(*o);
    }
  }
  SCRIPT_FUNCTION("vec3.minimize", vec3_minimize, lvec3_minimize)

  static const luaL_Reg fvec[] = {
    {"__gc", GCMethod<vec3> }
    , {"__index", lvec3_index }
    , {"__newindex", lvec3_newindex }
    , {"__unm", lvec3_unm }
    , {"__add", lvec3_add }
    , {"__sub", lvec3_sub }
    , {"__mul", lvec3_mul }
    , {"__div", lvec3_div }
    , {"length", lvec3_length }
    , {"length2", lvec3_length2 }
    , {"zero", lvec3_zero}
    , {"set", lvec3_set }
    , {"maximize", lvec3_maximize }
    , {"minimize", lvec3_minimize }
    , {NULL, NULL}
  };

  // add:
  // toString
  // dot, cross
  // angles
  // normalize

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
