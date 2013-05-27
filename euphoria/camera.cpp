// Euphoria - Copyright (c) Gustav

#include "euphoria/camera.h"

#include <cassert>

Camera::Camera()
  : fov(45.0f)
  , aspect(800.0f / 600)
  , znear(0.1f)
  , zfar(100.0f) {
  updateProjection();
}

void Camera::updateProjection() {
  assert(this);
  cml::matrix_perspective_xfov_LH(projection, fov, aspect, znear, zfar,
                                  cml::z_clip_zero);
}

const mat44 Camera::getProjection() const {
  assert(this);
  return projection;
}
