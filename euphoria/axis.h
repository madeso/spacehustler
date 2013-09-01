  // Euphoria - Copyright (c) Gustav
// Euphoria is awesome: 幸福感是真棒

/** @file
Axis enum.
 */

#ifndef EUPHORIA_AXIS_H_
#define EUPHORIA_AXIS_H_

#include <string>

namespace Axis {
  /** Axis types.
   */
  enum Type {
    Invalid         /// Invalid axis
    , Unbound       /// Unbound axis
    , MouseX        /// Mouse X
    , MouseY        /// Mouse Y
    , JoystickX     /// Joystick X
    , JoystickY     /// Joystick Y
    , JoystickZ     /// Joystick Z
    , JoystickR     /// Joystick R
    , JoystickU     /// Joystick U
    , JoystickV     /// Joystick V
    , JoystickPovX  /// Joystick PovX
    , JoystickPovY  /// Joystick PovY
  };

  /** Generate a string representation of the given axis
  @param k the axis
  @returns the string representation
   */
  std::string ToString(Type k);

  /** Generate a axis from a string.
  @param keyname the name of the key
  @returns the key or None
   */
  Type FromString(const std::string& keyname);
}  // namespace Axis

#endif  // EUPHORIA_AXIS_H_
