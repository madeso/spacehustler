// Euphoria - Copyright (c) Gustav

/** @file
Math related code.
 */

#ifndef EUPHORIA_MATH_H_
#define EUPHORIA_MATH_H_

#include "cml/cml.h"

namespace euphoria {

/** @defgroup math Math functions and types
@{
 */

// cpplint complain that we don't include <vector> but we use cml::vector
//   here so <vector> isn't needed

/** A integer vector in 2 dimensions.
 */
typedef cml::vector<int,  // NOLINT(build/include_what_you_use)
                    cml::fixed<2> > Vec2i;

/** A vector in 2 dimensions.
 */
typedef cml::vector<float,  // NOLINT(build/include_what_you_use)
                    cml::fixed<2> > Vec2;

/** A vector in 3 dimensions.
 */
typedef cml::vector<float,  // NOLINT(build/include_what_you_use)
                    cml::fixed<3> > Vec3;

/** A vector in 4 dimensions.
 */
typedef cml::vector<float,  // NOLINT(build/include_what_you_use)
                    cml::fixed<4> > Vec4;

/** A 4 x 4 matrix.
 */
typedef cml::matrix<float, cml::fixed<4, 4>, cml::col_basis, cml::col_major>
    Mat44;

/** A quaternion.
 */
typedef cml::quaternion<float, cml::fixed<>, cml::vector_first,
                        cml::negative_cross> Quat;

/** Generate a zero vector.
@returns the following vector (0,0,0)
 */
Vec3 CreateZeroedVec3();

/** Create a translation matrix.
@param v the vector containing the translation.
@returns the translation matrix.
 */
Mat44 CreateMat44(const Vec3& v);

/** Create a rotation matrix.
@param q the quaternion contining the rotation.
@returns the rotation matrix.
 */
Mat44 CreateMat44(const Quat& q);

/** Create a identity matrix.
@returns the identity matrix.
 */
Mat44 CreateIdentityMat44();

/** Combine a translation and a rotation into a translation/rototation matrix.
@param v the translation.
@param q the rotation.
@returns the combined matrix.
 */
Mat44 CreateMat44(const Vec3& v, const Quat& q);

/** Create a identity rotation.
@returns the identity rotation.
 */
Quat CreateIdentityQuat();

/** @} */

}  // namespace euphoria

#endif  // EUPHORIA_MATH_H_
