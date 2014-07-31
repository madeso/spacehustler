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
  @param width the width of the rendering area
  @param height the height of the rendering area
   */
  Camera(int width, int height);

  /** Gets the view matrix.
  @returns the view matrix.
   */
  const Mat44& view() const;

  /** Sets the view matrix.
  @param view the view matrix.
   */
  void set_view(const Mat44& view);

  /** Get the projection matrix.
  @return the projection matrix.
   */
  const Mat44 projection() const;

  /** Set the projection matrix.
  @param projection the new projection matrix.
   */
  void set_projection(const Mat44& projection);

  /** Set the new Field of view.
  @param fov the new FOV.
   */
  void set_fov(float fov);

  /** Changes the new/far planes.
  @param near the new near plane position.
  @param far the new far plane position.
   */
  void SetNearFar(float near, float far);

 private:
  Mat44 view_;
  float fov_;
  float aspect_;
  float znear_;
  float zfar_;

  Mat44 projection_;

 private:
  void UpdateProjection();
};

#endif  // EUPHORIA_CAMERA_H_
