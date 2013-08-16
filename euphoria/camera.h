// Euphoria - Copyright (c) Gustav

/** @file
Camera related code.
 */

#ifndef EUPHORIA_CAMERA_H_
#define EUPHORIA_CAMERA_H_

#include "euphoria/math.h"

/** A camera in a world.
@see World
 */
class Camera {
  public:
    /** Constructs a new camera.
     */
    Camera();

    /** The view matrix.
     */
    mat44 view;

    /** Get the projection matrix.
    @return the projection matrix.
     */
    const mat44 GetProjection() const;

    /** Set the new Field of view.
    @param newfov the new FOV.
     */
    void SetFov(float newfov);

    /** Changes the new/far planes.
    @param near the new near plane position.
    @param far the new far plane position.
     */
    void SetNearFar(float near, float far);

  private:
    float fov_;
    float aspect_;
    float znear_;
    float zfar_;

    mat44 projection_;

  private:
    void UpdateProjection();
};

#endif  // EUPHORIA_CAMERA_H_
