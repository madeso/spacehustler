// Euphoria - Copyright (c) Gustav

/** @file
Contains scalar math function.
 */

#ifndef EUPHORIA_SCALAR_H_
#define EUPHORIA_SCALAR_H_

namespace euphoria {

/// \defgroup math1 1-D math operations
/// @{

/** Tests if two floats are equal.
@param lhs one float
@param rhs the other float
@returns true if they are equal, false if not.
 */
bool IsEqual(float lhs, float rhs);

/** Tests if a float is close to zero.
@param r the float
@returns true if it is close to zero, false if not
 */
bool IsZero(float r);

/** Gets the value, or zero if it's close.
@param r the value
@returns the value or zero
 */
float ZeroOrValue(float r);

/** Calculates the sign as a positive or a negative int.
@param r the value
@returns 1 if r is greater than 0, -1 if not.
@see Abs()
*/
const int SignOf(const float r);

/** Linear interpolation.
@param f the starting point
@param scale the interpolation value, between 0 and 1
@param t the end point
@returns the interpolated value
 */
const float Lerp(const float f, float scale, const float t);

/** Performs a single interpolating step to transform a old value to a new
value.
Larger smoothing values result in a smoother but less responsive path

Example:
@code mousex = Curve( ( MousePos.X - OldMousePos.X ) * Sensitivity(), mousex, 6
); @endcode

@param target is the target value
@param current is the current value
@param smoothing is the smoothing value > 0, the lower, the less smooth

@returns The smoothed result.
*/
const float Curve(const float target, const float current,
                  const float smoothing);

/** Calculates the square of the argument.
@param r the value
@returns the square of the value
@see Sqrt()
*/
const float Square(const float r);

/** Calculates the square-root of the argument.
@param r the value
@returns the square root of the value
@see Square()
*/
const float Sqrt(const float r);

/** Calculates the absolute value of the argument.
@param r the value
@return the absolute value
@see SignOf()
*/
const float Abs(const float r);

/** Calculates the minimum of two values
@param lhs one float
@param rhs the other float
@returns the minimum value
@see Max()
@see KeepWithin()
*/
const float Min(const float lhs, const float rhs);

/** Calculates the maximum of two values.
@param lhs one float
@param rhs the other float
@returns the maximum value
@see Min()
@see KeepWithin()
*/
const float Max(const float lhs, const float rhs);

/** Transforms a value from one range to the 0-1 range.
This function does not limit the value, so if it's below the lower bound it
will be below 0.
@param L the lower bound of the range
@param v the value
@param U the upper bound of the range
@returns The transformed value.
@see From01()
@see Remap()
@see KeepWithin()
*/
const float To01(const float L, const float v, const float U);

/** Transforms a value from the 0-1 range to another range.
This function does not limit he value, so if it's below 0 the result will be
below the lower bound.
@param L the lower bound of the range
@param v the value
@param U the upper bound of the range
@returns The transformed value
@see To01()
@see KeepWithin()
@see Remap()
*/
const float From01(const float L, const float v, const float U);

/** Remaps/transforms from one range to another.
This function does not limit it's input, so if the value is outside the
original range, it will be outside the new range.
@param ol old lower range
@param ou old upper range
@param v the value
@param nl new lower range
@param nu new upper range
@returns The transformed value
@see From01()
@see To01()
@see KeepWithin()
*/
const float Remap(const float ol, const float ou, const float v, const float nl,
                  const float nu);

/** Calculate smooth value suitable for infinite looping.
@param min is the minimum output value
@param value is a float that indicates what to return
0 means minimum, 0.5 means maximum and 1 is minimum again, the values in between
follow a sinus/cosinus curve doing a full 360.
@param max is the maximum output value
@returns a value that lies between min and max
*/
const float Get360Angular(const float min, const float value, const float max);

/** Keeps a value within a minimum and a maximum.
Limits it to the range if it gets outside.
@param min is the minimum value
@param v is the value to limit
@param max is the maximum value
@returns the limited value
@see Min()
@see Max()
@see Wrap()
@see IsWithin()
*/
const float KeepWithin(const float min, const float v, const float max);

/** Checks whether a value is within a range, being at the border is not ok.
@param min the minimum value
@param c the value
@param max the maximum value
@returns true if the value is within, false if not
@see KeepWithin()
*/
const bool IsWithin(const float min, const float c, const float max);

/** Checks whether a value is within a range, being at the border is ok.
@param min the minimum value
@param c the value
@param max the maximum value
@returns true if the value is within, false if not
@see KeepWithin()
*/
const bool IsWithinInclusive(const float min, const float c, const float max);

/** Keeps a value within a range.
Wraps it to the others side if it gets outside.
@param min is the lower range
@param v is the value to be wrapped
@param max is the upper range
@returns the wrapped value
@see KeepWithin()
@see Wrapi()
@see IncrementAndWrap()
*/
const float Wrap(const float min, const float v, const float max);

/** Keeps a value within a range.
Wraps it to the others side if it gets outside.
@param min is the lower range
@param v is the value to be wrapped
@param max is the upper range
@returns the wrapped value
@see KeepWithin()
@see Wrap()
@see IncrementAndWrapi()
*/
const int Wrapi(const int min, const int v, const int max);

/** rounds a value to the nearest nice value.
If the granularity is 1 the function rounds to the closest integer, at .5 the
closest half integer, at 2 even integers etc...
@param num the number to round
@param gran the granularity
@returns the rounded value
*/
const float Round(const float num, const float gran);

/** Increment and wrap a value.
@param min the minimum value
@param current the value to change
@param change how much to change it
@param max the maximum value
@returns return number of wraps (with sign)
@see Wrap()
*/
const int IncrementAndWrap(const float min, float* current, const float change,
                           const float max);

/** Increment and wrap a value.
@param min the minimum value
@param current the value to change
@param change how much to change it
@param max the maximum value
@returns return number of wraps (with sign)
@see Wrapi()
*/
const int IncrementAndWrapi(const int min, int* current, const int change,
                            const int max);

/** The mathematical contant pi.
@returns pi
 */
const float Pi();

/** The mathematical contant pi/2.
@returns pi/2
*/
const float HalfPi();

/// @}

}  // namespace euphoria

#endif  // EUPHORIA_SCALAR_H_
