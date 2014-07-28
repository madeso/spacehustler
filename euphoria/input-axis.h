// Euphoria - Copyright (c) Gustav

/** @file
Axis enum.
 */

#ifndef EUPHORIA_INPUT_AXIS_H_
#define EUPHORIA_INPUT_AXIS_H_

#include <string>

/** Sign type
 */
enum class Sign {
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
Sign ToSign(const std::string& signname);

/** Axis types.
 */
enum class Axis {
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
std::string ToString(Axis k);

/** Generate a axis from a string.
@param keyname the name of the key
@returns the key or None
 */
Axis ToAxis(const std::string& keyname);

#endif  // EUPHORIA_INPUT_AXIS_H_
