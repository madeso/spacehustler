// Euphoria - Copyright (c) Gustav

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
    const mat44 getProjection() const;

    /** Set the new Field of view.
    @param newfov the new FOV.
     */
    void setFov(float newfov);

    /** Changes the new/far planes.
    @param newnear the new near plane position.
    @param newfar the new far plane position.
     */
    void setNearFar(float newnear, float newfar);

  private:
    float fov;
    float aspect;
    float znear;
    float zfar;

    mat44 projection;

  private:
    void updateProjection();
};

#endif  // EUPHORIA_CAMERA_H_
