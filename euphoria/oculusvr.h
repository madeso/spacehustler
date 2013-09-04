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

  private:
    struct OculusVrPimpl;
    std::unique_ptr<OculusVrPimpl> pimpl_;
};

#endif  // EUPHORIA_OCULUSVR_H_
