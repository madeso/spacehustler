  // Euphoria - Copyright (c) Gustav
// Euphoria is awesome: 幸福感是真棒

/** @file
Oculus VR related integration code.
 */

#ifndef EUPHORIA_OCULUSVR_H_
#define EUPHORIA_OCULUSVR_H_

#include <boost/noncopyable.hpp>
#include <memory>

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
    EyeSetup(float w, float h, float x, float y, const mat44& projection,
             const mat44& view_adjust);

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
    const mat44& projection() const;

    /** Get the translation to be applied to view matrix.
    @return the translation to be applied to view matrix.
     */
    const mat44& view_adjust() const;

  private:
    float w_;
    float h_;
    float x_;
    float y_;
    mat44 projection_;
    mat44 view_adjust_;
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
    const vec4& get_distortion() const;

    /** Gets the scale.
    @returns the scale
     */
    float get_scale() const;

    /** Gets the center offset.
    @returns the center offset
     */
    const vec2& get_center_offset() const;

    /** Gets the chromatic aberration values from the oculus sdk.
    @returns the the chromatic aberration values from the oculus sdk
     */
    const vec4 get_chromatic_aberration() const;

  private:
    struct OculusVrPimpl;
    std::unique_ptr<OculusVrPimpl> pimpl_;
};

#endif  // EUPHORIA_OCULUSVR_H_
