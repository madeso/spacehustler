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
  void cvec3_create(ScriptParams* p) {
    float x = 0;
    float y = 0;
    float z = 0;

    if (ScriptOverload(p) << &x << &y << &z) {
      // -- Description: Creates a 3 dimensional vector
      // -- Arguments:
      // -- Number The X coordinate
      // -- Number The Y coordinate
      // -- Number The Z coordinate
      // -- Returns: vec3 a vector
      lua_pushobject(p->ReturnFullUserData(), vec3)(x, y, z);
    } else if (ScriptOverload(p)) {
      // -- Description: Creates a vector at origin
      // -- Returns: vec3 a vector placed at origin
      lua_pushobject(p->ReturnFullUserData(), vec3)(0, 0, 0);
    }
  }
  SCRIPT_FUNCTION("cvec3.create", cvec3_create, lcvec3_create)

  // -- Function: cardinal
  void cvec3_cardinal(ScriptParams* p) {
    int i = 0;

    if (ScriptOverload(p) << &i) {
      // -- Description: Create a cardinal vector.
      // -- Arguments:
      // -- Number The cardinal vector index, pass 1 to create (1,0,0) etc.
      // -- Returns: The created vector.
      vec3* r = lua_pushobject(p->ReturnFullUserData(), vec3)(0, 0, 0);
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

  // -- Type: vec3

  // -- Operator: .
  void vec3_index(ScriptParams* p) {
    vec3* a;
    std::string key;
    int index;
    if (ScriptOverload(p) << mFullUserData(vec3, &a) << &key) {
      // -- Description: Get or sets a value from the vector
      // -- Arguments:
      // -- String the name, x for x, y for z for z
      // -- Returns: Number the value of x, y or z
      assert(a);
      if (key == "x") {
        p->Return((*a)[0]);
      } else if (key == "y") {
        p->Return((*a)[1]);
      } else if (key == "z") {
        p->Return((*a)[2]);
      } else {
        throw "Invalid key";
      }
    } else if (ScriptOverload(p) << mFullUserData(vec3, &a) << &index) {
      // -- Description: Getor sets a value from the vector
      // -- Arguments:
      // -- Number the index between 1 and 3 that indicates the x,y or z value
      // -- Returns: Number The value
      assert(a);
      if (index <= 0) {
        throw "Invalid index";
      }
      if (index > 3) {
        throw "Invalid index";
      }
      p->Return((*a)[index - 1]);
    }
  }
  SCRIPT_FUNCTION("vec3.index", vec3_index, lvec3_index)

  // documented above
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

  // -- Operator: unary -
  void vec3_unm(ScriptParams* p) {
    vec3* a;
    if (ScriptOverload(p) << mFullUserData(vec3, &a)) {
      // -- Description: Returns a negative representation of the vector
      // -- Arguments: vec3 the vector to negate
      // -- Returns: vec3 the negated vector
      assert(a);
      vec3* r = lua_pushobject(p->ReturnFullUserData(), vec3)(0, 0, 0);
      *r = -*a;
    }
  }
  SCRIPT_FUNCTION("vec3.unm", vec3_unm, lvec3_unm)

  // -- Operator: +
  void vec3_add(ScriptParams* p) {
    vec3* a;
    vec3* b;
    float f;
    if (ScriptOverload(p) << mFullUserData(vec3, &a)
        << mFullUserData(vec3, &b)) {
      assert(a);
      assert(b);
      // -- Description: Add two vectors.
      // -- Arguments:
      // -- vec3 the left operand
      // -- vec3 the right operand
      // -- Returns: vec3 the sum of the 2 vectors.
      vec3* r = lua_pushobject(p->ReturnFullUserData(), vec3)(0, 0, 0);
      *r = *a + *b;
    } else if (ScriptOverload(p) << mFullUserData(vec3, &a)
               << &f) {
      assert(a);
      // -- Description: Add a vector and a number.
      // -- Arguments:
      // -- vec3 the left operand
      // -- Number the number to add
      // -- Returns: vec3 the sum of the vector and the float
      vec3* r = lua_pushobject(p->ReturnFullUserData(), vec3)(0, 0, 0);
      *r = *a + vec3(f, f, f);
    }
    if (ScriptOverload(p) << &f << mFullUserData(vec3, &a)) {
      assert(a);
      vec3* r = lua_pushobject(p->ReturnFullUserData(), vec3)(0, 0, 0);
      *r = vec3(f, f, f) + *a;
    }
  }
  SCRIPT_FUNCTION("vec3.add", vec3_add, lvec3_add)

  // -- Operator: -
  void vec3_sub(ScriptParams* p) {
    vec3* a;
    vec3* b;
    float f;
    if (ScriptOverload(p) << mFullUserData(vec3, &a)
        << mFullUserData(vec3, &b)) {
      assert(a);
      assert(b);
      // -- Description: Substract two vectors.
      // -- Arguments:
      // -- vec3 the left operand
      // -- vec3 the right operand
      // -- Returns: vec3 the result.
      vec3* r = lua_pushobject(p->ReturnFullUserData(), vec3)(0, 0, 0);
      *r = *a - *b;
    } else if (ScriptOverload(p) << mFullUserData(vec3, &a)
               << &f) {
      assert(a);
      // -- Description: Subtract a vector and a number.
      // -- Arguments:
      // -- vec3 the left operand
      // -- Number the number to subtract
      // -- Returns: vec3 the result
      vec3* r = lua_pushobject(p->ReturnFullUserData(), vec3)(0, 0, 0);
      *r = *a - vec3(f, f, f);
    } else if (ScriptOverload(p) << &f << mFullUserData(vec3, &a)) {
      assert(a);
      vec3* r = lua_pushobject(p->ReturnFullUserData(), vec3)(0, 0, 0);
      *r = vec3(f, f, f) - *a;
    }
  }
  SCRIPT_FUNCTION("vec3.sub", vec3_sub, lvec3_sub)

  // -- Operator: *
  void vec3_mul(ScriptParams* p) {
    vec3* a;
    float f;
    if (ScriptOverload(p) << mFullUserData(vec3, &a)
        << &f) {
      assert(a);
      // -- Description: Multiply a vector and a number.
      // -- Arguments:
      // -- vec3 the left operand
      // -- float the number to multiply with
      // -- Returns: vec3 the result
      vec3* r = lua_pushobject(p->ReturnFullUserData(), vec3)(0, 0, 0);
      *r = *a * f;
    }
    if (ScriptOverload(p) << &f << mFullUserData(vec3, &a)) {
      assert(a);
      vec3* r = lua_pushobject(p->ReturnFullUserData(), vec3)(0, 0, 0);
      *r = *a * f;
    }
  }
  SCRIPT_FUNCTION("vec3.mul", vec3_mul, lvec3_mul)

  // -- Operator: /
  void vec3_div(ScriptParams* p) {
    vec3* a;
    float f;
    if (ScriptOverload(p) << mFullUserData(vec3, &a)
        << &f) {
      assert(a);
      // -- Description: Divide a vector and a number.
      // -- Arguments:
      // -- vec3 the left operand
      // -- float the number to divide
      // -- Returns: vec3 the result
      vec3* r = lua_pushobject(p->ReturnFullUserData(), vec3)(0, 0, 0);
      *r = *a / f;
    }
  }
  SCRIPT_FUNCTION("vec3.div", vec3_div, lvec3_div)

  // -- Function: length
  void vec3_length(ScriptParams* p) {
    vec3* a;
    if (ScriptOverload(p) << mFullUserData(vec3, &a)) {
      assert(a);
      // -- Description: Gets the length of the vector.
      // -- Arguments: vec3 the vector
      // -- Returns: Number the length
      p->Return(a->length());
    }
  }
  SCRIPT_FUNCTION("vec3.length", vec3_length, lvec3_length)

  // -- Function: length2
  void vec3_length2(ScriptParams* p) {
    vec3* a;
    if (ScriptOverload(p) << mFullUserData(vec3, &a)) {
      assert(a);
      // -- Description: Returns the squared length.
      // -- Arguments: vec3 the vector
      // -- Returns: Number the length squared
      p->Return(a->length_squared());
    }
  }
  SCRIPT_FUNCTION("vec3.length2", vec3_length2, lvec3_length2)

  // -- Function: zero
  void vec3_zero(ScriptParams* p) {
    vec3* a;
    if (ScriptOverload(p) << mFullUserData(vec3, &a)) {
      assert(a);
      // -- Description: Sets all values to 0
      // -- Arguments: vec3 the vector to clear.
      a->zero();
    }
  }
  SCRIPT_FUNCTION("vec3.zero", vec3_zero, lvec3_zero)

  // -- Function: set
  void vec3_set(ScriptParams* p) {
    vec3* a;
    float x;
    float y;
    float z;
    float all;
    if (ScriptOverload(p) << mFullUserData(vec3, &a) << &x << &y << &z) {
      assert(a);
      // -- Description: sets the value of all values
      // -- Arguments:
      // -- Number the X
      // -- Number the Y
      // -- Number the Z
      a->set(x, y, z);
    } else if (ScriptOverload(p) << mFullUserData(vec3, &a) << &all) {
      assert(a);
      // -- Description: sets the values to a single value
      // -- Arguments: Number the the value of X, Y and Z
      a->set(all, all, all);
    }
  }
  SCRIPT_FUNCTION("vec3.set", vec3_set, lvec3_set)

  // -- Function: maximize
  void vec3_maximize(ScriptParams* p) {
    vec3* a;
    float x;
    float y;
    float z;
    float all;
    vec3* o;
    if (ScriptOverload(p) << mFullUserData(vec3, &a) << &x << &y << &z) {
      // -- Description: Sets the vector to the pairwise maximum.
      // -- Arguments:
      // -- vec3 the vector
      // -- Number the X
      // -- Number the Y
      // -- Number the Z
      assert(a);
      a->maximize(vec3(x, y, z));
    } else if (ScriptOverload(p) << mFullUserData(vec3, &a) << &all) {
      assert(a);
      // -- Description: Sets the vector to the pairwise maximum.
      // -- Arguments:
      // -- vec3 the vector
      // -- Number the X, Y and Z
      a->maximize(vec3(all, all, all));
    } else if (ScriptOverload(p) << mFullUserData(vec3, &a)
               << mFullUserData(vec3, &o)) {
      assert(a);
      // -- Description: Sets the vector to the pairwise maximum.
      // -- Arguments:
      // -- vec3 the vector
      // -- vec3 the other vector
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
      // -- Description: Sets the vector to the pairwise minimum.
      // -- Arguments:
      // -- vec3 the vector
      // -- Number the X
      // -- Number the Y
      // -- Number the Z
      a->minimize(vec3(x, y, z));
    } else if (ScriptOverload(p) << mFullUserData(vec3, &a) << &all) {
      assert(a);
      // -- Description: Sets the vector to the pairwise minimum.
      // -- Arguments:
      // -- vec3 the vector
      // -- Number the X, Y and Z
      a->minimize(vec3(all, all, all));
    } else if (ScriptOverload(p) << mFullUserData(vec3, &a)
               << mFullUserData(vec3, &o)) {
      assert(a);
      // -- Description: Sets the vector to the pairwise minimum.
      // -- Arguments:
      // -- vec3 the vector
      // -- vec3 the other vector
      a->minimize(*o);
    }
  }
  SCRIPT_FUNCTION("vec3.minimize", vec3_minimize, lvec3_minimize)

  void vec3_toString(ScriptParams* p) {
    vec3* a;
    if (ScriptOverload(p) << mFullUserData(vec3, &a)) {
      assert(a);
      // -- Function: toString
      // -- Description: Generate a string representation of the vector.
      // -- Arguments:
      // -- vec3 the vector
      // -- Returns: string the resulting string
      p->Return(Str() << "(" << (*a)[0] << ", " << (*a)[1]  << ", "
                << (*a)[2] << ")");
    }
  }
  SCRIPT_FUNCTION("vec3.toString", vec3_toString, lvec3_toString)

  void vec3_dot(ScriptParams* p) {
    vec3* a;
    vec3* b;
    if (ScriptOverload(p) << mFullUserData(vec3, &a)
        << mFullUserData(vec3, &b)) {
      assert(a);
      assert(b);
      // -- Function: dot
      // -- Description: Calculates the dot product
      // -- Arguments:
      // -- vec3 the lhs
      // -- vec3 the rhs
      // -- Returns: Number the dot product between lhs and rhs
      p->Return(dot(*a, *b));
    }
  }
  SCRIPT_FUNCTION("vec3.dot", vec3_dot, lvec3_dot)

  void vec3_cross(ScriptParams* p) {
    vec3* a;
    vec3* b;
    if (ScriptOverload(p) << mFullUserData(vec3, &a)
        << mFullUserData(vec3, &b)) {
      assert(a);
      assert(b);
      // -- Function: cross
      // -- Description: Calculates the cross product
      // -- Arguments:
      // -- vec3 the lhs
      // -- vec3 the rhs
      // -- Returns: vec3 the cross product between lhs and rhs
      vec3* r = lua_pushobject(p->ReturnFullUserData(), vec3)(0, 0, 0);
      *r = cross(*a, *b);
    }
  }
  SCRIPT_FUNCTION("vec3.cross", vec3_cross, lvec3_cross)

  void vec3_crossnorm(ScriptParams* p) {
    /// @todo No check for small magnitude is made; if the input vectors are
    /// parallel or nearly parallel the result may be invalid.
    vec3* a;
    vec3* b;
    if (ScriptOverload(p) << mFullUserData(vec3, &a)
        << mFullUserData(vec3, &b)) {
      assert(a);
      assert(b);
      // -- Function: crossnorm
      // -- Returns: Number the normalized cross product of two vec3
      // -- Arguments:
      // -- vec3 the lhs
      // -- vec3 the rhs
      // -- Returns: Number the normalized cross product of lhs and rhs
      vec3* r = lua_pushobject(p->ReturnFullUserData(), vec3)(0, 0, 0);
      *r = unit_cross(*a, *b);
    }
  }
  SCRIPT_FUNCTION("vec3.crossnorm", vec3_crossnorm, lvec3_crossnorm)

  void vec3_rotate(ScriptParams* p) {
    vec3* a;
    vec3* axis;
    float angle;
    if (ScriptOverload(p) << mFullUserData(vec3, &a)
        << mFullUserData(vec3, &axis) << &angle) {
      assert(a);
      assert(axis);
      // -- Function: rotate
      // -- Description: Rotate a vector around a unitlength axis
      // -- Arguments:
      // -- vec3 the vector
      // -- vec3 the axis
      // -- Number the angle in radians
      vec3* r = lua_pushobject(p->ReturnFullUserData(), vec3)(0, 0, 0);
      *r = rotate_vector(*a, *axis, angle);
    }
  }
  SCRIPT_FUNCTION("vec3.rotate", vec3_rotate, lvec3_rotate)

  void vec3_angle(ScriptParams* p) {
    vec3* a;
    vec3* b;
    vec3* c;
    if (ScriptOverload(p) << mFullUserData(vec3, &a)
        << mFullUserData(vec3, &b)) {
      assert(a);
      assert(b);
      // -- Function: angle
      // -- Description: Returnsd the unsigned angle between 2 vectors of
      // -- arbitary length
      // -- Arguments:
      // -- vec3 the first vector
      // -- vec3 the second vector
      // -- Returns: Number the angle in radians
      const float angle = unsigned_angle(*a, *b);
      p->Return(angle);
    } else if (ScriptOverload(p) << mFullUserData(vec3, &a)
               << mFullUserData(vec3, &b)
               << mFullUserData(vec3, &c)) {
      assert(a);
      assert(b);
      assert(c);
      // -- Function: angle
      // -- Description: Returns the signed angle between 2 vectors relative
      // -- to a third.
      // -- Arguments:
      // -- vec3 the first vector
      // -- vec3 the second vector
      // -- vec3 the reference vector
      // -- Returns:
      // -- Number the singed angle in radians, measured from the first vector
      const float angle = signed_angle(*a, *b, *c);
      p->Return(angle);
    }
  }
  SCRIPT_FUNCTION("vec3.angle", vec3_angle, lvec3_angle)

  void vec3_normalize(ScriptParams* p) {
    vec3* a;
    if (ScriptOverload(p) << mFullUserData(vec3, &a)) {
      assert(a);
      // -- Function: normalize
      // -- Description: Normalize the vector.
      // -- Arguments:
      // -- vec3 the vector to normalize
      /// @todo No checking for near-zero magnitude is performed.
      *a = normalize(*a);
    }
  }
  SCRIPT_FUNCTION("vec3.normalize", vec3_normalize, lvec3_normalize)

  void vec3_getnormalized(ScriptParams* p) {
    vec3* a;
    if (ScriptOverload(p) << mFullUserData(vec3, &a)) {
      assert(a);
      // -- Function: getNormalized
      // -- Description: Gets the normalized vector without modifying
      // -- the argument
      // -- Arguments:
      // -- vec3 the vector to normalize
      // -- Returns: vec3 the normalized vector
      /// @todo No checking for near-zero magnitude is performed.
      vec3* r = lua_pushobject(p->ReturnFullUserData(), vec3)(0, 0, 0);
      *r = normalize(*a);
    }
  }
  SCRIPT_FUNCTION("vec3.getnormalized", vec3_getnormalized, lvec3_getnormalized)

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
    , {"__tostring", lvec3_toString }
    , {"dot", lvec3_dot }
    , {"cross", lvec3_cross }
    , {"crossnorm", lvec3_crossnorm }
    , {"rotate", lvec3_rotate }
    , {"normalize", lvec3_normalize }
    , {"getNormalized", lvec3_getnormalized }
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
