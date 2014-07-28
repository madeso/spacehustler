// Euphoria - Copyright (c) Gustav

/** @file
Axis enum.
 */

#ifndef EUPHORIA_ANGLE_H_
#define EUPHORIA_ANGLE_H_

/** A angle in both degrees and radians. Internally stored as a radian.
*/
class Angle {
 public:
  /** Convert the current value to degrees.
  @returns the angle in degrees
  */
  const float InDegrees() const;

  /** Gets the current value in radians.
  @returns the angle in radians
  */
  const float InRadians() const;

  /** Creates a angle from degrees.
  @param deg the angle in degrees
  @returns a new angle
  */
  static const Angle FromDegrees(float deg);

  /** Creates a angle from radians.
  @param rad the angle in radians
  @returns a new angle
  */
  static const Angle FromRadians(float rad);

  /** Wraps the angle around 360 degrees.
  @see GetWrapped(const Angle&)
  */
  void Wrap();

  /** Angle addition.
  @param rhs the angle to add
  */
  void operator+=(const Angle& rhs);

  /** Angle subtraction.
  @param rhs the angle to subtract
  */
  void operator-=(const Angle& rhs);

  /** Angle scaling.
  @param r the scale
  */
  void operator*=(const float r);

 private:
  explicit Angle(float rad);
  float radians_;
};

/** Global operator.
@param lhs the left argument
@param rhs the right argument
@returns the result
 */
const Angle operator+(const Angle& lhs, const Angle& rhs);

/** Global operator.
@param lhs the left argument
@param rhs the right argument
@returns the result
*/
const Angle operator-(const Angle& lhs, const Angle& rhs);

/** Global operator.
@param lhs the left argument
@param rhs the right argument
@returns the result
*/
const Angle operator*(const Angle& lhs, const float rhs);

/** Global operator.
@param lhs the left argument
@param rhs the right argument
@returns the result
*/
const Angle operator*(const float rhs, const Angle& lhs);

namespace AngleOperations {
/** Sine.
Ratio of the length of the side opposite the given angle to the length of the
hypotenuse of a right-angled triangle.
@param ang the angle
@returns the result
 */
const float Sin(const Angle& ang);

/** Cosine.
Ratio of the adjacent side to the hypotenuse of a right-angled triangle.
@param ang the angle
@returns the result
*/
const float Cos(const Angle& ang);

/** Tangent.
Ratio of the opposite to the adjacent side of a right-angled triangle.
@param ang the angle
@returns the result
*/
const float Tan(const Angle& ang);

/** Arc sine.
The inverse function of the sine. The angle that has a sine equal to a given
number.
@param v the value
@returns the resulting angle
*/
const Angle Asin(const float v);

/** Arc cosine.
The inverse function of the cosine. The angle that has a cosine equal to a
given number.
@param v the value
@returns the resulting angle
*/
const Angle Acos(const float v);

/** Arc tangent.
The inverse function of the tangent. The angle that has a tangent equal to
a given number.
@param v the value
@returns the resulting angle
*/
const Angle Atan(const float v);

/** Returns a wrapped angle.
@param a the angle to wrap
@returns the wrapped angle
 */
const Angle GetWrapped(const Angle& a);

/** Given a percentage, returns a angle.
@param percent the percentage
@returns the angle
 */
const Angle FromPercentOf360(const float percent);

/** The zero angle.
@returns the zero angle.
 */
const Angle Zero();
}  // namespace AngleOperations

#endif  // EUPHORIA_ANGLE_H_
