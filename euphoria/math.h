// Euphoria - Copyright (c) Gustav

#ifndef EUPHORIA_MATH_H_
#define EUPHORIA_MATH_H_


#include "cml/cml.h"

// cpplint complain that we don't include <vector> but we use cml::vector
//   here so <vector> isn't needed
typedef cml::vector < float,  // NOLINT(build/include_what_you_use)
        cml::fixed<3> > vec3;

typedef cml::matrix < float, cml::fixed<4, 4>,
        cml::col_basis, cml::col_major > mat44;

typedef cml::quaternion < float, cml::fixed<>, cml::vector_first,
        cml::negative_cross > quat;

vec3 cvec3zero();

mat44 cmat44(const vec3& v);
mat44 cmat44(const quat& q);

mat44 cmat44(const vec3& v, const quat& q);

#endif  // EUPHORIA_MATH_H_
