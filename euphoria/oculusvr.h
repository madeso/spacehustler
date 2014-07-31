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

/** Oculus VR data for a eye.
 */
class EyeSetup {
 public:
  /** Constructor.
  @param w the width of the viewport
  @param h the height of the viewport
  @param x the X of the viewport
  @param y the Y of the viewport
  @param projection the projection matrix used with this eye.
  @param view_adjust the translation to be applied to view matrix.
   */
  EyeSetup(float w, float h, float x, float y, const Mat44& projection,
           const Mat44& view_adjust);

  /** Get the width of the viewport
  @return the width of the viewport
   */
  float w() const;

  /** Get the height of the viewport
  @return the height of the viewport
   */
  float h() const;

  /** Get the X of the viewport
  @return the X of the viewport
   */
  float x() const;

  /** Get the Y of the viewport
  @return the Y of the viewport
   */
  float y() const;

  /** Get the projection matrix used with this eye.
  @return the projection matrix used with this eye.
   */
  const Mat44& projection() const;

  /** Get the translation to be applied to view matrix.
  @return the translation to be applied to view matrix.
   */
  const Mat44& view_adjust() const;

 private:
  float w_;
  float h_;
  float x_;
  float y_;
  Mat44 projection_;
  Mat44 view_adjust_;
};

/** A util class for integration of the oculus vr kit.
 */
class OculusVr : boost::noncopyable {
 public:
  OculusVr();
  ~OculusVr();

  /** Get the eye configuration for the left eye.
   @returns the left eye configuration
   */
  const EyeSetup LeftEye();

  /** Get the eye configuration for the right eye.
   @returns the right eye configuration
   */
  const EyeSetup RightEye();

  /** Gets the oculus distortion constants.
  @returns the oculus distortion constants.
   */
  const Vec4& GetDistortion() const;

  /** Gets the scale.
  @returns the scale
   */
  float GetScale() const;

  /** Gets the orientation of the rift.
  @returns the orientation of the rift.
  @param predict_orientation true if the orientation should be predicted
   */
  Quat GetOrientation(bool predict_orientation) const;

  /** Resets the orientation of the rift.
   */
  void ResetOrientation();

  /** Gets the center offset.
  @returns the center offset
   */
  const Vec2& GetCenterOffset() const;

  /** Gets the chromatic aberration values from the oculus sdk.
  @returns the the chromatic aberration values from the oculus sdk
   */
  const Vec4 GetChromaticAberration() const;

  /** Gets the detection message.
  @returns the detection message.
   */
  const std::string GetDetectionMessage() const;

 private:
  class OculusVrPimpl;
  std::unique_ptr<OculusVrPimpl> pimpl_;
};

#endif  // EUPHORIA_OCULUSVR_H_
