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
namespace cvec3 {
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
    LUA_PUSH_OBJECT(p->ReturnFullUserData(), Vec3)(x, y, z);
  } else if (ScriptOverload(p)) {
    // -- Description: Creates a vector at origin
    // -- Returns: vec3 a vector placed at origin
    LUA_PUSH_OBJECT(p->ReturnFullUserData(), Vec3)(0, 0, 0);
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
    // -- Returns: vec3 The created vector.
    Vec3* r = LUA_PUSH_OBJECT(p->ReturnFullUserData(), Vec3)(0, 0, 0);
    if (i > 3) {
      const std::string error = Str() << "Invalid cardinal index";
      throw error;
    }
    if (i <= 0) {
      const std::string error = Str() << "Invalid cardinal index";
      throw error;
    }
    r->cardinal(i - 1);
  }
}
SCRIPT_FUNCTION("cvec3.cardinal", cvec3_cardinal, lcvec3_cardinal)
// lol
}  // namespace cvec3 NOLINT

static const luaL_Reg vec3lib[] = {{"create", cvec3::lcvec3_create},
                                   {"cardinal", cvec3::lcvec3_cardinal},
                                   {NULL, NULL}};

namespace lvec3 {
// -- Type: vec3
// -- Operator: .
void vec3_index(ScriptParams* p) {
  Vec3* a;
  std::string key;
  int index;
  if (ScriptOverload(p) << FULL_USER_DATA(Vec3, &a) << &key) {
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
      const std::string error = Str() << "Invalid key";
      throw error;
    }
  } else if (ScriptOverload(p) << FULL_USER_DATA(Vec3, &a) << &index) {
    // -- Description: Getor sets a value from the vector
    // -- Arguments:
    // -- Number the index between 1 and 3 that indicates the x,y or z value
    // -- Returns: Number The value
    assert(a);
    if (index <= 0) {
      const std::string error = Str() << "Invalid index";
      throw error;
    }
    if (index > 3) {
      const std::string error = Str() << "Invalid index";
      throw error;
    }
    p->Return((*a)[index - 1]);
  }
}
SCRIPT_FUNCTION("vec3.index", vec3_index, lvec3_index)

// documented above
void vec3_newindex(ScriptParams* p) {
  Vec3* a;
  std::string key;
  int index;
  float value;
  if (ScriptOverload(p) << FULL_USER_DATA(Vec3, &a) << &key << &value) {
    assert(a);
    if (key == "x") {
      (*a)[0] = value;
    } else if (key == "y") {
      (*a)[1] = value;
    } else if (key == "z") {
      (*a)[2] = value;
    } else {
      const std::string error = Str() << "Invalid key";
      throw error;
    }
  } else if (ScriptOverload(p) << FULL_USER_DATA(Vec3, &a) << &index
                               << &value) {
    assert(a);
    if (index <= 0) {
      const std::string error = Str() << "Invalid index";
      throw error;
    }
    if (index > 3) {
      const std::string error = Str() << "Invalid index";
      throw error;
    }
    (*a)[index - 1] = value;
  }
}
SCRIPT_FUNCTION("vec3.newindex", vec3_newindex, lvec3_newindex)

// -- Operator: unary -
void vec3_unm(ScriptParams* p) {
  Vec3* a;
  if (ScriptOverload(p) << FULL_USER_DATA(Vec3, &a)) {
    // -- Description: Returns a negative representation of the vector
    // -- Arguments: vec3 the vector to negate
    // -- Returns: vec3 the negated vector
    assert(a);
    Vec3* r = LUA_PUSH_OBJECT(p->ReturnFullUserData(), Vec3)(0, 0, 0);
    *r = -*a;
  }
}
SCRIPT_FUNCTION("vec3.unm", vec3_unm, lvec3_unm)

// -- Operator: +
void vec3_add(ScriptParams* p) {
  Vec3* a;
  Vec3* b;
  float f;
  if (ScriptOverload(p) << FULL_USER_DATA(Vec3, &a)
                        << FULL_USER_DATA(Vec3, &b)) {
    assert(a);
    assert(b);
    // -- Description: Add two vectors.
    // -- Arguments:
    // -- vec3 the left operand
    // -- vec3 the right operand
    // -- Returns: vec3 the sum of the 2 vectors.
    Vec3* r = LUA_PUSH_OBJECT(p->ReturnFullUserData(), Vec3)(0, 0, 0);
    *r = *a + *b;
  } else if (ScriptOverload(p) << FULL_USER_DATA(Vec3, &a) << &f) {
    assert(a);
    // -- Description: Add a vector and a number.
    // -- Arguments:
    // -- vec3 the left operand
    // -- Number the number to add
    // -- Returns: vec3 the sum of the vector and the float
    Vec3* r = LUA_PUSH_OBJECT(p->ReturnFullUserData(), Vec3)(0, 0, 0);
    *r = *a + Vec3(f, f, f);
  } else if (ScriptOverload(p) << &f << FULL_USER_DATA(Vec3, &a)) {
    assert(a);
    Vec3* r = LUA_PUSH_OBJECT(p->ReturnFullUserData(), Vec3)(0, 0, 0);
    *r = Vec3(f, f, f) + *a;
  }
}
SCRIPT_FUNCTION("vec3.add", vec3_add, lvec3_add)

// -- Operator: -
void vec3_sub(ScriptParams* p) {
  Vec3* a;
  Vec3* b;
  float f;
  if (ScriptOverload(p) << FULL_USER_DATA(Vec3, &a)
                        << FULL_USER_DATA(Vec3, &b)) {
    assert(a);
    assert(b);
    // -- Description: Substract two vectors.
    // -- Arguments:
    // -- vec3 the left operand
    // -- vec3 the right operand
    // -- Returns: vec3 the result.
    Vec3* r = LUA_PUSH_OBJECT(p->ReturnFullUserData(), Vec3)(0, 0, 0);
    *r = *a - *b;
  } else if (ScriptOverload(p) << FULL_USER_DATA(Vec3, &a) << &f) {
    assert(a);
    // -- Description: Subtract a vector and a number.
    // -- Arguments:
    // -- vec3 the left operand
    // -- Number the number to subtract
    // -- Returns: vec3 the result
    Vec3* r = LUA_PUSH_OBJECT(p->ReturnFullUserData(), Vec3)(0, 0, 0);
    *r = *a - Vec3(f, f, f);
  } else if (ScriptOverload(p) << &f << FULL_USER_DATA(Vec3, &a)) {
    assert(a);
    Vec3* r = LUA_PUSH_OBJECT(p->ReturnFullUserData(), Vec3)(0, 0, 0);
    *r = Vec3(f, f, f) - *a;
  }
}
SCRIPT_FUNCTION("vec3.sub", vec3_sub, lvec3_sub)

// -- Operator: *
void vec3_mul(ScriptParams* p) {
  Vec3* a;
  float f;
  if (ScriptOverload(p) << FULL_USER_DATA(Vec3, &a) << &f) {
    assert(a);
    // -- Description: Multiply a vector and a number.
    // -- Arguments:
    // -- vec3 the left operand
    // -- float the number to multiply with
    // -- Returns: vec3 the result
    Vec3* r = LUA_PUSH_OBJECT(p->ReturnFullUserData(), Vec3)(0, 0, 0);
    *r = *a * f;
  } else if (ScriptOverload(p) << &f << FULL_USER_DATA(Vec3, &a)) {
    assert(a);
    Vec3* r = LUA_PUSH_OBJECT(p->ReturnFullUserData(), Vec3)(0, 0, 0);
    *r = *a * f;
  }
}
SCRIPT_FUNCTION("vec3.mul", vec3_mul, lvec3_mul)

// -- Operator: /
void vec3_div(ScriptParams* p) {
  Vec3* a;
  float f;
  if (ScriptOverload(p) << FULL_USER_DATA(Vec3, &a) << &f) {
    assert(a);
    // -- Description: Divide a vector and a number.
    // -- Arguments:
    // -- vec3 the left operand
    // -- float the number to divide
    // -- Returns: vec3 the result
    Vec3* r = LUA_PUSH_OBJECT(p->ReturnFullUserData(), Vec3)(0, 0, 0);
    *r = *a / f;
  }
}
SCRIPT_FUNCTION("vec3.div", vec3_div, lvec3_div)

// -- Function: length
void vec3_length(ScriptParams* p) {
  Vec3* a;
  if (ScriptOverload(p) << FULL_USER_DATA(Vec3, &a)) {
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
  Vec3* a;
  if (ScriptOverload(p) << FULL_USER_DATA(Vec3, &a)) {
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
  Vec3* a;
  if (ScriptOverload(p) << FULL_USER_DATA(Vec3, &a)) {
    assert(a);
    // -- Description: Sets all values to 0
    // -- Arguments: vec3 the vector to clear.
    a->zero();
  }
}
SCRIPT_FUNCTION("vec3.zero", vec3_zero, lvec3_zero)

// -- Function: set
void vec3_set(ScriptParams* p) {
  Vec3* a;
  float x;
  float y;
  float z;
  float all;
  if (ScriptOverload(p) << FULL_USER_DATA(Vec3, &a) << &x << &y << &z) {
    assert(a);
    // -- Description: sets the value of all values
    // -- Arguments:
    // -- Number the X
    // -- Number the Y
    // -- Number the Z
    a->set(x, y, z);
  } else if (ScriptOverload(p) << FULL_USER_DATA(Vec3, &a) << &all) {
    assert(a);
    // -- Description: sets the values to a single value
    // -- Arguments: Number the the value of X, Y and Z
    a->set(all, all, all);
  }
}
SCRIPT_FUNCTION("vec3.set", vec3_set, lvec3_set)

// -- Function: maximize
void vec3_maximize(ScriptParams* p) {
  Vec3* a;
  float x;
  float y;
  float z;
  float all;
  Vec3* o;
  if (ScriptOverload(p) << FULL_USER_DATA(Vec3, &a) << &x << &y << &z) {
    // -- Description: Sets the vector to the pairwise maximum.
    // -- Arguments:
    // -- vec3 the vector
    // -- Number the X
    // -- Number the Y
    // -- Number the Z
    assert(a);
    a->maximize(Vec3(x, y, z));
  } else if (ScriptOverload(p) << FULL_USER_DATA(Vec3, &a) << &all) {
    assert(a);
    // -- Description: Sets the vector to the pairwise maximum.
    // -- Arguments:
    // -- vec3 the vector
    // -- Number the X, Y and Z
    a->maximize(Vec3(all, all, all));
  } else if (ScriptOverload(p) << FULL_USER_DATA(Vec3, &a)
                               << FULL_USER_DATA(Vec3, &o)) {
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
  Vec3* a;
  float x;
  float y;
  float z;
  float all;
  Vec3* o;
  if (ScriptOverload(p) << FULL_USER_DATA(Vec3, &a) << &x << &y << &z) {
    assert(a);
    // -- Description: Sets the vector to the pairwise minimum.
    // -- Arguments:
    // -- vec3 the vector
    // -- Number the X
    // -- Number the Y
    // -- Number the Z
    a->minimize(Vec3(x, y, z));
  } else if (ScriptOverload(p) << FULL_USER_DATA(Vec3, &a) << &all) {
    assert(a);
    // -- Description: Sets the vector to the pairwise minimum.
    // -- Arguments:
    // -- vec3 the vector
    // -- Number the X, Y and Z
    a->minimize(Vec3(all, all, all));
  } else if (ScriptOverload(p) << FULL_USER_DATA(Vec3, &a)
                               << FULL_USER_DATA(Vec3, &o)) {
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
  Vec3* a;
  if (ScriptOverload(p) << FULL_USER_DATA(Vec3, &a)) {
    assert(a);
    // -- Function: toString
    // -- Description: Generate a string representation of the vector.
    // -- Arguments:
    // -- vec3 the vector
    // -- Returns: string the resulting string
    p->Return(Str() << "(" << (*a)[0] << ", " << (*a)[1] << ", " << (*a)[2]
                    << ")");
  }
}
SCRIPT_FUNCTION("vec3.toString", vec3_toString, lvec3_toString)

void vec3_dot(ScriptParams* p) {
  Vec3* a;
  Vec3* b;
  if (ScriptOverload(p) << FULL_USER_DATA(Vec3, &a)
                        << FULL_USER_DATA(Vec3, &b)) {
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
  Vec3* a;
  Vec3* b;
  if (ScriptOverload(p) << FULL_USER_DATA(Vec3, &a)
                        << FULL_USER_DATA(Vec3, &b)) {
    assert(a);
    assert(b);
    // -- Function: cross
    // -- Description: Calculates the cross product
    // -- Arguments:
    // -- vec3 the lhs
    // -- vec3 the rhs
    // -- Returns: vec3 the cross product between lhs and rhs
    Vec3* r = LUA_PUSH_OBJECT(p->ReturnFullUserData(), Vec3)(0, 0, 0);
    *r = cross(*a, *b);
  }
}
SCRIPT_FUNCTION("vec3.cross", vec3_cross, lvec3_cross)

void vec3_crossnorm(ScriptParams* p) {
  /// @todo No check for small magnitude is made; if the input vectors are
  /// parallel or nearly parallel the result may be invalid.
  Vec3* a;
  Vec3* b;
  if (ScriptOverload(p) << FULL_USER_DATA(Vec3, &a)
                        << FULL_USER_DATA(Vec3, &b)) {
    assert(a);
    assert(b);
    // -- Function: crossnorm
    // -- Returns: Number the normalized cross product of two vec3
    // -- Arguments:
    // -- vec3 the lhs
    // -- vec3 the rhs
    // -- Returns: Number the normalized cross product of lhs and rhs
    Vec3* r = LUA_PUSH_OBJECT(p->ReturnFullUserData(), Vec3)(0, 0, 0);
    *r = unit_cross(*a, *b);
  }
}
SCRIPT_FUNCTION("vec3.crossnorm", vec3_crossnorm, lvec3_crossnorm)

void vec3_rotate(ScriptParams* p) {
  Vec3* a;
  Vec3* axis;
  float angle;
  if (ScriptOverload(p) << FULL_USER_DATA(Vec3, &a)
                        << FULL_USER_DATA(Vec3, &axis) << &angle) {
    assert(a);
    assert(axis);
    // -- Function: rotate
    // -- Description: Rotate a vector around a unitlength axis
    // -- Arguments:
    // -- vec3 the vector
    // -- vec3 the axis
    // -- Number the angle in radians
    Vec3* r = LUA_PUSH_OBJECT(p->ReturnFullUserData(), Vec3)(0, 0, 0);
    *r = rotate_vector(*a, *axis, angle);
  }
}
SCRIPT_FUNCTION("vec3.rotate", vec3_rotate, lvec3_rotate)

void vec3_angle(ScriptParams* p) {
  Vec3* a;
  Vec3* b;
  Vec3* c;
  if (ScriptOverload(p) << FULL_USER_DATA(Vec3, &a)
                        << FULL_USER_DATA(Vec3, &b)) {
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
  } else if (ScriptOverload(p) << FULL_USER_DATA(Vec3, &a)
                               << FULL_USER_DATA(Vec3, &b)
                               << FULL_USER_DATA(Vec3, &c)) {
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
  Vec3* a;
  if (ScriptOverload(p) << FULL_USER_DATA(Vec3, &a)) {
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
  Vec3* a;
  if (ScriptOverload(p) << FULL_USER_DATA(Vec3, &a)) {
    assert(a);
    // -- Function: getNormalized
    // -- Description: Gets the normalized vector without modifying
    // -- the argument
    // -- Arguments:
    // -- vec3 the vector to normalize
    // -- Returns: vec3 the normalized vector
    /// @todo No checking for near-zero magnitude is performed.
    Vec3* r = LUA_PUSH_OBJECT(p->ReturnFullUserData(), Vec3)(0, 0, 0);
    *r = normalize(*a);
  }
}
SCRIPT_FUNCTION("vec3.getnormalized", vec3_getnormalized, lvec3_getnormalized)
}  // namespace lvec3 NOLINT

static const luaL_Reg fvec[] = {{"__gc", GCMethod<Vec3>},
                                {"__index", lvec3::lvec3_index},
                                {"__newindex", lvec3::lvec3_newindex},
                                {"__unm", lvec3::lvec3_unm},
                                {"__add", lvec3::lvec3_add},
                                {"__sub", lvec3::lvec3_sub},
                                {"__mul", lvec3::lvec3_mul},
                                {"__div", lvec3::lvec3_div},
                                {"length", lvec3::lvec3_length},
                                {"length2", lvec3::lvec3_length2},
                                {"zero", lvec3::lvec3_zero},
                                {"set", lvec3::lvec3_set},
                                {"maximize", lvec3::lvec3_maximize},
                                {"minimize", lvec3::lvec3_minimize},
                                {"__tostring", lvec3::lvec3_toString},
                                {"dot", lvec3::lvec3_dot},
                                {"cross", lvec3::lvec3_cross},
                                {"crossnorm", lvec3::lvec3_crossnorm},
                                {"rotate", lvec3::lvec3_rotate},
                                {"normalize", lvec3::lvec3_normalize},
                                {"getNormalized", lvec3::lvec3_getnormalized},
                                {NULL, NULL}};

void createmeta(lua_State* state, const std::string& name,
                const luaL_Reg* functions) {
  luaL_newmetatable(state, name.c_str());
  lua_pushvalue(state, -1);
  lua_setfield(state, -2, "__index");
  luaL_setfuncs(state, functions, 0);
  lua_pop(state, 1);
}

LUAMOD_API int scriptlib_vec3_open(lua_State* state) {
  luaL_newlib(state, vec3lib);
  createmeta(state, "Vec3", fvec);
  return 1;
}

namespace cquat {
// -- Module: cquat
// -- Function: identity
void cquat_identity(ScriptParams* p) {
  if (ScriptOverload(p)) {
    // -- Description: Creates a identity quaternion
    // -- Returns: quat the quaternion
    Quat* q = LUA_PUSH_OBJECT(p->ReturnFullUserData(), Quat)();
    q->identity();
  }
}
SCRIPT_FUNCTION("cquat.identity", cquat_identity, identity)

// -- Function: worldx
void cquat_worldx(ScriptParams* p) {
  float angle = 0.0f;
  if (ScriptOverload(p) << &angle) {
    // -- Description: Creates a rotation quaternion around world x
    // -- Arguments:
    // -- Number the angle in radians
    // -- Returns: quat the quaternion
    Quat* q = LUA_PUSH_OBJECT(p->ReturnFullUserData(), Quat)();
    quaternion_rotation_world_axis(*q, 0, angle);
  }
}
SCRIPT_FUNCTION("cquat.worldx", cquat_worldx, worldx)

// -- Function: worldy
void cquat_worldy(ScriptParams* p) {
  float angle = 0.0f;
  if (ScriptOverload(p) << &angle) {
    // -- Description: Creates a rotation quaternion around world y
    // -- Arguments:
    // -- Number the angle in radians
    // -- Returns: quat the identity quaternion
    Quat* q = LUA_PUSH_OBJECT(p->ReturnFullUserData(), Quat)();
    quaternion_rotation_world_axis(*q, 1, angle);
  }
}
SCRIPT_FUNCTION("cquat.worldy", cquat_worldy, worldy)

// -- Function: worldz
void cquat_worldz(ScriptParams* p) {
  float angle = 0.0f;
  if (ScriptOverload(p) << &angle) {
    // -- Description: Creates a rotation quaternion around world z
    // -- Arguments:
    // -- Number the angle in radians
    // -- Returns: quat the quaternion
    Quat* q = LUA_PUSH_OBJECT(p->ReturnFullUserData(), Quat)();
    quaternion_rotation_world_axis(*q, 2, angle);
  }
}
SCRIPT_FUNCTION("cquat.worldz", cquat_worldz, worldz)

// -- Function: axisangle
void cquat_axisangle(ScriptParams* p) {
  Vec3* axis = 0;
  float angle = 0.0f;
  if (ScriptOverload(p) << FULL_USER_DATA(Vec3, &axis) << &angle) {
    assert(axis);
    // -- Description: Creates a rotation quaternion from axis and angle
    // -- Arguments:
    // -- vec3 the axis
    // -- Number the angle in radians
    // -- Returns: quat the quaternion
    Quat* q = LUA_PUSH_OBJECT(p->ReturnFullUserData(), Quat)();
    quaternion_rotation_axis_angle(*q, *axis, angle);
  }
}
SCRIPT_FUNCTION("cquat.axisangle", cquat_axisangle, axisangle)

Quat crquat(int index, float val) {
  Quat t;
  quaternion_rotation_world_axis(t, index, val);
  return t;
}

// -- Function: yawpitchroll
void cquat_yawpitchroll(ScriptParams* p) {
  float yaw = 0.0f;
  float pitch = 0.0f;
  float roll = 0.0f;
  if (ScriptOverload(p) << &yaw << &pitch << &roll) {
    // -- Description:
    // -- Creates a rotation quaternion from yaw, pitch and roll
    // -- Arguments:
    // -- vec3 the axis
    // -- Number the yaw
    // -- Number the pitch
    // -- Number the roll
    // -- Returns: quat the quaternion
    Quat* q = LUA_PUSH_OBJECT(p->ReturnFullUserData(), Quat)();
    const Quat qroll = crquat(2, roll);
    const Quat qpitch = crquat(0, pitch);
    const Quat qyaw = crquat(1, yaw);
    *q = qroll * qpitch * qyaw;
  }
}
SCRIPT_FUNCTION("cquat.yawpitchroll", cquat_yawpitchroll, yawpitchroll)
}  // namespace cquat NOLINT

static const luaL_Reg quatlib[] = {{"identity", cquat::identity},
                                   {"worldx", cquat::worldx},
                                   {"worldy", cquat::worldy},
                                   {"worldz", cquat::worldz},
                                   {"axisangle", cquat::axisangle},
                                   {"yawpitchroll", cquat::yawpitchroll},
                                   {NULL, NULL}};

namespace lquat {
// -- Type: quat

// -- Operator: *
void quat_mul(ScriptParams* p) {
  Quat* a = 0;
  Quat* b = 0;

  if (ScriptOverload(p) << FULL_USER_DATA(Quat, &a)
                        << FULL_USER_DATA(Quat, &b)) {
    assert(a);
    assert(b);
    // -- Description: Multiplies two quaternions
    // -- Arguments:
    // -- quat The first quaternion
    // -- quat The second quaternion
    // -- Returns: quat The multiplied quaternion
    Quat* ret = ReturnQuat(p);
    *ret = *a * *b;
  }
}
SCRIPT_FUNCTION("quat.mul", quat_mul, mul)

// -- Function: getx
void quat_getx(ScriptParams* p) {
  Quat* q = 0;

  if (ScriptOverload(p) << FULL_USER_DATA(Quat, &q)) {
    assert(q);
    // -- Description: Gets the x base vector
    // -- Arguments:
    // -- quat The quaternion
    // -- Returns: vec3 the basis vector
    Vec3* ret = LUA_PUSH_OBJECT(p->ReturnFullUserData(), Vec3)();
    *ret = quaternion_get_basis_vector(*q, 0);
  }
}
SCRIPT_FUNCTION("quat.getx", quat_getx, getx)

// -- Function: gety
void quat_gety(ScriptParams* p) {
  Quat* q = 0;

  if (ScriptOverload(p) << FULL_USER_DATA(Quat, &q)) {
    assert(q);
    // -- Description: Gets the y base vector
    // -- Arguments:
    // -- quat The quaternion
    // -- Returns: vec3 the basis vector
    Vec3* ret = LUA_PUSH_OBJECT(p->ReturnFullUserData(), Vec3)();
    *ret = quaternion_get_basis_vector(*q, 1);
  }
}
SCRIPT_FUNCTION("quat.gety", quat_gety, gety)

// -- Function: getz
void quat_getz(ScriptParams* p) {
  Quat* q = 0;

  if (ScriptOverload(p) << FULL_USER_DATA(Quat, &q)) {
    assert(q);
    // -- Description: Gets the z base vector
    // -- Arguments:
    // -- quat The quaternion
    // -- Returns: vec3 the basis vector
    Vec3* ret = LUA_PUSH_OBJECT(p->ReturnFullUserData(), Vec3)();
    *ret = quaternion_get_basis_vector(*q, 2);
  }
}
SCRIPT_FUNCTION("quat.getz", quat_getz, getz)

// -- Operator: unary -
void quat_unm(ScriptParams* p) {
  Quat* q = 0;
  Quat* junk = 0;

  if (ScriptOverload(p) << FULL_USER_DATA(Quat, &q)
                        << FULL_USER_DATA(Quat, &junk)) {
    assert(q);
    // -- Description: Gets the conjugate of the quaterion
    // -- Arguments:
    // -- quat The quaternion
    // -- Returns: quat the conjugate
    Quat* ret = LUA_PUSH_OBJECT(p->ReturnFullUserData(), Quat)();
    *ret = *q;
    ret->conjugate();
  }
}
SCRIPT_FUNCTION("quat.unm", quat_unm, unm)
}  // namespace lquat NOLINT

static const luaL_Reg fquat[] = {{"__gc", GCMethod<Quat>},
                                 {"__mul", lquat::mul},
                                 {"getx", lquat::getx},
                                 {"gety", lquat::gety},
                                 {"getz", lquat::getz},
                                 {"__unm", lquat::unm},
                                 {NULL, NULL}};

LUAMOD_API int scriptlib_quat_open(lua_State* state) {
  luaL_newlib(state, quatlib);
  createmeta(state, "Quat", fquat);
  return 1;
}
}  // namespace

void RegisterScriptLib(lua_State* state) {
  luaL_requiref(state, "cvec3", scriptlib_vec3_open, 1);
  lua_pop(state, 1);
  luaL_requiref(state, "cquat", scriptlib_quat_open, 1);
  lua_pop(state, 1);
}

Quat* ReturnQuat(ScriptParams* params) {
  return LUA_PUSH_OBJECT(params->ReturnFullUserData(), Quat)();
}

Vec3* ReturnVec3(ScriptParams* params) {
  return LUA_PUSH_OBJECT(params->ReturnFullUserData(), Vec3)();
}
