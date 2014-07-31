// Euphoria - Copyright (c) Gustav

#include "euphoria/math.h"
#include <cassert>

Vec3 cvec3zero() {
  Vec3 ret;
  ret.zero();
  return ret;
}

Mat44 cmat44(const Vec3& v) {
  Mat44 ret;
  cml::matrix_translation(ret, v);
  return ret;
}

Mat44 cmat44(const Quat& q) {
  Mat44 ret;
  cml::matrix_rotation_quaternion(ret, q);
  return ret;
}

Mat44 cmat44(const Vec3& v, const Quat& q) { return cmat44(v) * cmat44(q); }

Quat cquatIdent() {
  Quat r;
  r.identity();
  return r;
}
