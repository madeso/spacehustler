// Euphoria - Copyright (c) Gustav

/** @file
Camera related code.
 */

#ifndef EUPHORIA_CAMERA_H_
#define EUPHORIA_CAMERA_H_

#include "euphoria/math.h"

namespace euphoria {

class NearFar {
 public:
  NearFar(float near, float far);
  float near() const;
  float far() const;

 private:
  const float near_;
  const float far_;
};

/** A camera in a world.
@see World
 */
class Camera {
 public:
  /** Constructs a new camera.
   */
  Camera(const Mat44& view, const Mat44& projection);

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

 private:
  Mat44 view_;
  Mat44 projection_;
};

Camera CreateCameraPerspective(float fov, int width, int height,
                               const NearFar& nearfar);
Camera CreateCameraOrtho(int width, int height, const NearFar& nearfar);

}  // namespace euphoria

#endif  // EUPHORIA_CAMERA_H_
