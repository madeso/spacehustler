// Euphoria - Copyright (c) Gustav

/** @file
Range enum.
 */

#ifndef EUPHORIA_INPUT_RANGE_H_
#define EUPHORIA_INPUT_RANGE_H_

#include <string>

/** Definition of the kind of range.
 */
enum class Range {
  Invalid,
  Infinite,
  Within01,
  WithinNegative1Positive1
};

/** Generate a string representation of the given range
@param k the range
@returns the string representation
 */
std::string ToString(Range k);

/** Generate a range from a string.
@param name the name of the range
@returns the range or None
 */
Range ToRange(const std::string& name);

#endif  // EUPHORIA_INPUT_RANGE_H_
