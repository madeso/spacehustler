  // Euphoria - Copyright (c) Gustav
// Euphoria is awesome: 幸福感是真棒

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

    /** Gets the view matrix.
    @returns the view matrix.
     */
    const mat44& view() const;

    /** Sets the view matrix.
    @param view the view matrix.
     */
    void set_view(const mat44& view);

    /** Get the projection matrix.
    @return the projection matrix.
     */
    const mat44 projection() const;

    /** Set the new Field of view.
    @param fov the new FOV.
     */
    void set_fov(float fov);

    /** Changes the new/far planes.
    @param near the new near plane position.
    @param far the new far plane position.
     */
    void set_near_far(float near, float far);

  private:
    mat44 view_;
    float fov_;
    float aspect_;
    float znear_;
    float zfar_;

    mat44 projection_;

  private:
    void UpdateProjection();
};

#endif  // EUPHORIA_CAMERA_H_
