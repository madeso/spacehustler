// Euphoria - Copyright (c) Gustav

#include "euphoria/camera.h"

#include <cassert>

namespace euphoria {

NearFar::NearFar(float near, float far) : near_(near), far_(far) {
  assert(this);
}

float NearFar::near() const {
  assert(this);
  return near_;
}

float NearFar::far() const {
  assert(this);
  return far_;
}

//////////////////////////////////////////////////////////////////////////
Camera::Camera(const Mat44& view, const Mat44& projection, int width,
               int height)
    : view_(view), projection_(projection), width_(width), height_(height) {}

const Mat44 Camera::projection() const {
  assert(this);
  return projection_;
}

void Camera::set_projection(const Mat44& projection) {
  assert(this);
  projection_ = projection;
}

const Mat44& Camera::view() const {
  assert(this);
  return view_;
}

void Camera::set_view(const Mat44& view) {
  assert(this);
  view_ = view;
}

int Camera::width() const {
  assert(this);
  return width_;
}

void Camera::set_width(int width) {
  assert(this);
  width_ = width;
}

int Camera::height() const {
  assert(this);
  return height_;
}

void Camera::set_height(int height) {
  assert(this);
  height_ = height;
}

float CreateAspect(int width, int height) {
  return static_cast<float>(width) / height;
}

Mat44 CreateCameraPerspective(float fov, int width, int height,
                              const NearFar& nearfar) {
  Mat44 projection;
  cml::matrix_perspective_xfov_RH(projection, fov, CreateAspect(width, height),
                                  nearfar.near(), nearfar.far(),
                                  cml::z_clip_zero);
  return projection;
}

Mat44 CreateCameraOrtho(int width, int height, const NearFar& nearfar) {
  Mat44 projection;
  cml::matrix_orthographic_RH(projection, static_cast<float>(width),
                              static_cast<float>(height), nearfar.near(),
                              nearfar.far(), cml::z_clip_zero);
  return projection;
}

}  // namespace euphoria
