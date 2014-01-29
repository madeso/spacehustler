// Euphoria - Copyright (c) Gustav

/** @file
Axis enum.
 */

#ifndef EUPHORIA_ANGLE_H_
#define EUPHORIA_ANGLE_H_

/** A angle in both degrees and radians. Internally stored as a radian.
*/
struct Angle {
 public:
  /** Convert the current value to degrees.
  @returns the angle in degrees
  */
  const float inDegrees() const;

  /** Gets the current value in radians.
  @returns the angle in radians
  */
  const float inRadians() const;

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
  void wrap();

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
  float mRad;
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
/** Sinus.
@param ang the angle
@returns the result
 */
const float Sin(const Angle& ang);

/** Cosinus.
@param ang the angle
@returns the result
*/
const float Cos(const Angle& ang);

/** Tangens.
@param ang the angle
@returns the result
*/
const float Tan(const Angle& ang);

/** Asinus.
@param v the value
@returns the resulting angle
*/
const Angle Asin(const float v);

/** Acosinus.
@param v the value
@returns the resulting angle
*/
const Angle Acos(const float v);

/** Atangens.
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
