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

  /** Get the translation to be applied to view matrix.
  @return the translation to be applied to view matrix.
   */
  const Mat44& view_adjust() const;

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

  /** Get the eye configuration for the left eye.
   @returns the left eye configuration
   */
  const EyeSetup LeftEye();

  bool IsHmdDetected();

  /** Get the eye configuration for the right eye.
   @returns the right eye configuration
   */
  const EyeSetup RightEye();
 private:
  class OculusVrPimpl;
  std::unique_ptr<OculusVrPimpl> pimpl_;
};

#endif  // EUPHORIA_OCULUSVR_H_
