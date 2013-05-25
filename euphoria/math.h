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

#endif  // EUPHORIA_MATH_H_
