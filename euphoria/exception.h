// Euphoria - Copyright (c) Gustav

/** @file
Exception related code
 */

#ifndef EUPHORIA_EXCEPTION_H_
#define EUPHORIA_EXCEPTION_H_

#include <string>

namespace euphoria {

/** Function for grabbing the exception information.
This function is only useful when called with a unknown exception being caught.
@returns the exception message as a string
 */
std::string GrabExceptionInformation();

}  // namespace euphoria

#endif  // EUPHORIA_EXCEPTION_H_
