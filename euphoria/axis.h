// Euphoria - Copyright (c) Gustav

/** @file
Axis enum.
 */

#ifndef EUPHORIA_AXIS_H_
#define EUPHORIA_AXIS_H_

#include <string>

namespace Sign {
/** Sign type
 */
enum Type {
  Invalid  /// Invalid sign
  ,
  Positive  /// Positive sign
  ,
  Negative  /// Negative sign
  ,
  FullRange  /// Both positive and negative sign
};

/** Generate a sign from a string.
@param signname the name of the sign
@returns the sign or Invalid
 */
Type FromString(const std::string& signname);
}  // namespace Sign

namespace Axis {
/** Axis types.
 */
enum Type {
  Invalid  /// Invalid axis
  ,
  Unbound  /// Unbound axis
  ,
  X  /// Mouse X
  ,
  Y  /// Mouse Y
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
