  // Euphoria - Copyright (c) Gustav
// Euphoria is awesome: 幸福感是真棒

/** @file
Exception related code
 */

#ifndef EUPHORIA_EXCEPTION_H_
#define EUPHORIA_EXCEPTION_H_

#include <string>

/** Function for grabbing the exception information.
This function is only useful when called with a unknown exception being caught.
@returns the exception message as a string
 */
std::string GrabExceptionInformation();

#endif  // EUPHORIA_EXCEPTION_H_
