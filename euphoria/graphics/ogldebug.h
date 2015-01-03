// Euphoria - Copyright (c) Gustav

/** @file
Utility code for debugging OpenGL.
 */

#ifndef EUPHORIA_GRAPHICS_OGLDEBUG_H_
#define EUPHORIA_GRAPHICS_OGLDEBUG_H_

#include "euphoria/noncopyable.h"

namespace euphoria {

/** Utility class for registration for debugging OpenGL errors  and throwing
 * when such a error occurs.
 */
class OglDebug : NonCopyable {
 public:
  /** Register for errors.
  @param debug true if we should register, false if not
   */
  explicit OglDebug(bool debug);

  /** Deregister for errors.
   */
  ~OglDebug();

  /** Tests if the debugging feature is supported by the current driver.
  @return true if supported, false if not.
   */
  static bool IsSupported();

  /** Function for explicitly testing if OpenGl has detected any errors.
   */
  static void Verify();

 private:
  const bool debug_enabled_;
};

}  // namespace euphoria

#endif  // EUPHORIA_GRAPHICS_OGLDEBUG_H_
