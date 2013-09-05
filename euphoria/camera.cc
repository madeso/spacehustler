﻿  // Euphoria - Copyright (c) Gustav
// Euphoria is awesome: 幸福感是真棒

#include "euphoria/camera.h"

#include <cassert>

Camera::Camera(int width, int height)
  : fov_(45.0f)
  , aspect_(static_cast<float>(width) / height)
  , znear_(0.1f)
  , zfar_(100.0f) {
  view_ = cmat44(cvec3zero(), cquatIdent());
  UpdateProjection();
}

const mat44 Camera::projection() const {
  assert(this);
  return projection_;
}

void Camera::set_projection(const mat44& projection) {
  assert(this);
  projection_ = projection;
}

void Camera::set_fov(float fov) {
  assert(this);

  fov_ = fov;
  UpdateProjection();
}

const mat44& Camera::view() const {
  assert(this);
  return view_;
}

void Camera::set_view(const mat44& view) {
  assert(this);
  view_ = view;
}

void Camera::set_near_far(float near, float far) {
  assert(this);

  znear_ = near;
  zfar_ = far;
  UpdateProjection();
}

void Camera::UpdateProjection() {
  assert(this);
  cml::matrix_perspective_xfov_RH(projection_, fov_, aspect_, znear_, zfar_,
                                  cml::z_clip_zero);
}
