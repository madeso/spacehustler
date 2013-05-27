// Euphoria - Copyright (c) Gustav

#ifndef EUPHORIA_CAMERA_H_
#define EUPHORIA_CAMERA_H_

#include "euphoria/math.h"

class Camera {
  public:
    Camera();

    mat44 view;

    const mat44 getProjection() const;

  public:
    float fov;
    float aspect;
    float znear;
    float zfar;

    mat44 projection;

  private:
    void updateProjection();
};

#endif  // EUPHORIA_CAMERA_H_
