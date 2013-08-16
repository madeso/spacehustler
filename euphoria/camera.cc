// Euphoria - Copyright (c) Gustav

#include "euphoria/camera.h"

#include <cassert>

Camera::Camera()
  : fov_(45.0f)
  , aspect_(800.0f / 600)
  , znear_(0.1f)
  , zfar_(100.0f) {
  view = cmat44(cvec3zero(), cquatIdent());
  UpdateProjection();
}

const mat44 Camera::GetProjection() const {
  assert(this);
  return projection_;
}

void Camera::SetFov(float fov) {
  assert(this);

  fov_ = fov;
  UpdateProjection();
}

void Camera::SetNearFar(float near, float far) {
  assert(this);

  znear_ = near;
  zfar_ = far;
  UpdateProjection();
}

void Camera::UpdateProjection() {
  assert(this);
  cml::matrix_perspective_xfov_LH(projection_, fov_, aspect_, znear_, zfar_,
                                  cml::z_clip_zero);
}
