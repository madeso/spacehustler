// Euphoria - Copyright (c) Gustav

/** @file
Oculus VR related integration code.
 */

#ifndef EUPHORIA_OCULUSVR_H_
#define EUPHORIA_OCULUSVR_H_

#include <boost/noncopyable.hpp>
#include <memory>
#include <string>

#include "euphoria/math.h"
#include "euphoria/fbo.h"

/** Oculus VR data for a eye.
 */
class EyeSetup {
 public:
  /** Constructor.
  @param projection the projection matrix used with this eye.
  @param view_adjust the translation to be applied to view matrix.
   */
  EyeSetup(const Mat44& projection, const Mat44& view_adjust, int w, int h, bool mipmap);

  /** Get the projection matrix used with this eye.
  @return the projection matrix used with this eye.
   */
  const Mat44& projection() const;
  void set_projection(const Mat44& projection);

  /** Get the translation to be applied to view matrix.
  @return the translation to be applied to view matrix.
   */
  const Mat44& view_adjust() const;
  void set_view_adjust(const Mat44& view_adjust);

  Fbo& fbo();

  const Fbo& fbo() const;

 private:
  Mat44 projection_;
  Mat44 view_adjust_;
  Fbo fbo_;
};

/** A util class for integration of the oculus vr kit.
 */
class OculusVr : boost::noncopyable {
 public:
  OculusVr();
  ~OculusVr();

  bool Detect(bool detect_debug_device);

  bool IsHmdDetected();
  const Vec2i GetWindowSize() const;

  void Begin();
  int GetNumberOfEyes() const;
  EyeSetup& GetEyeIndex(int eyeIndex);
  void End();
 private:
  class OculusVrPimpl;
  std::unique_ptr<OculusVrPimpl> pimpl_;
};

#endif  // EUPHORIA_OCULUSVR_H_
