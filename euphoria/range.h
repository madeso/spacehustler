// Euphoria - Copyright (c) Gustav

/** @file
Range enum.
 */

#ifndef EUPHORIA_RANGE_H_
#define EUPHORIA_RANGE_H_

#include <string>

namespace Range {
/** Definition of the kind of range.
 */
enum Type {
  Invalid,
  Infinite,
  Within01,
  WithinNegative1Positive1
};
/** Generate a string representation of the given range
@param k the range
@returns the string representation
 */
std::string ToString(Type k);

/** Generate a range from a string.
@param name the name of the range
@returns the range or None
 */
Type FromString(const std::string& name);
};  // namespace Range

#endif  // EUPHORIA_RANGE_H_
