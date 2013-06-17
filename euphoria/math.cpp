// Euphoria - Copyright (c) Gustav

#include "euphoria/math.h"
#include <cassert>

vec3 cvec3zero() {
  vec3 ret;
  ret.zero();
  return ret;
}

mat44 cmat44(const vec3& v) {
  mat44 ret;
  cml::matrix_translation(ret, v);
  return ret;
}

mat44 cmat44(const quat& q) {
  mat44 ret;
  cml::matrix_rotation_quaternion(ret, q);
  return ret;
}

mat44 cmat44(const vec3& v, const quat& q) {
  return cmat44(v) * cmat44(q);
}
