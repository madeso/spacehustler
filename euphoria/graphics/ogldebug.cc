// Euphoria - Copyright (c) Gustav

#include "euphoria/graphics/ogldebug.h"

#include <string>
#include <stdexcept>

#include "euphoria/graphics/opengl.h"
#include "euphoria/str.h"

namespace euphoria {

/**
@throws a std::logic_error when a error has occurred.
 */
void OglDebug::Verify() {
  const GLenum err = glGetError();
  if (err != GL_NO_ERROR) {
    std::string name = "Unknown OpenGL error";
    std::string description;

    switch (err) {
#define ERR(x, d)    \
  case x:            \
    name = #x;       \
    description = d; \
    break
      ERR(GL_INVALID_ENUM,
          "An unacceptable value is specified for an enumerated argument."
          " The offending command is ignored"
          " and has no other side effect than to set the error flag.");
      ERR(GL_INVALID_VALUE,
          "A numeric argument is out of range."
          " The offending command is ignored"
          " and has no other side effect than to set the error flag.");
      ERR(GL_INVALID_OPERATION,
          "The specified operation is not allowed in the current state."
          " The offending command is ignored"
          " and has no other side effect than to set the error flag.");
      ERR(GL_INVALID_FRAMEBUFFER_OPERATION,
          "The framebuffer object is not complete. The offending command"
          " is ignored and has no other side effect than to set"
          " the error flag.");
      ERR(GL_OUT_OF_MEMORY,
          "There is not enough memory left to execute the command."
          " The state of the GL is undefined,"
          " except for the state of the error flags,"
          " after this error is recorded.");
      ERR(GL_STACK_UNDERFLOW,
          "An attempt has been made to perform an operation that would"
          " cause an internal stack to underflow.");
      ERR(GL_STACK_OVERFLOW,
          "An attempt has been made to perform an operation that would"
          " cause an internal stack to overflow.");
#undef ERR
    };

    throw std::logic_error(Str() << "Open GL error occurred " << name << ": "
                                 << description);
  }
}

void GLAPIENTRY MyDebugCallback(GLenum source, GLenum type, GLuint id,
                              GLenum severity, GLsizei length,
                              const GLchar* message, const GLvoid* userParam) {
  std::string mes = message;
  throw mes;
}

bool OglDebug::IsSupported() { return GLEW_ARB_debug_output == GL_TRUE; }

OglDebug::OglDebug(bool d) : debug_enabled_(d) {
  if (debug_enabled_) {
    if (IsSupported() == false) {
      const std::string error = Str() << "OpenGL debug NOT supported";
      throw error;
    }
    Verify();
    glDebugMessageCallbackARB(&MyDebugCallback, 0);
    Verify();
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
    Verify();
  }
}

OglDebug::~OglDebug() {
  if (debug_enabled_) {
    Verify();
    glDebugMessageCallbackARB(0, 0);
    Verify();
    glDisable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
    Verify();
  }
}
}  // namespace euphoria
