// Euphoria - Copyright (c) Gustav

#include "euphoria/math.h"
#include <cassert>

Vec3 CreateZeroedVec3() {
  Vec3 ret;
  ret.zero();
  return ret;
}

Mat44 CreateMat44(const Vec3& v) {
  Mat44 ret;
  cml::matrix_translation(ret, v);
  return ret;
}

Mat44 CreateMat44(const Quat& q) {
  Mat44 ret;
  cml::matrix_rotation_quaternion(ret, q);
  return ret;
}

Mat44 CreateIdentityMat44() {
  Mat44 ret;
  ret.identity();
  return ret;
}

Mat44 CreateMat44(const Vec3& v, const Quat& q) {
  return CreateMat44(v) * CreateMat44(q);
}

Quat CreateIdentityQuat() {
  Quat r;
  r.identity();
  return r;
}
