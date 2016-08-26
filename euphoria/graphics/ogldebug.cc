// Euphoria - Copyright (c) Gustav

#include "euphoria/graphics/ogldebug.h"

#include <string>
#include <stdexcept>
#include <euphoria/game.h>
#include <euphoria/log.h>

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

std::string ClassifySource(GLenum source) {
  switch(source) {
    case GL_DEBUG_SOURCE_API_ARB: return "API";
    case GL_DEBUG_SOURCE_SHADER_COMPILER_ARB: return "shader compiler";
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB: return "window system";
    case GL_DEBUG_SOURCE_THIRD_PARTY_ARB : return "third party";
    case GL_DEBUG_SOURCE_APPLICATION_ARB: return "application";
    case GL_DEBUG_SOURCE_OTHER_ARB: return "other";
    default: return "unknown";
  }
}

std::string ClassifyType(GLenum type) {
  switch(type) {
    case GL_DEBUG_TYPE_ERROR_ARB: return "error";
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB: return "deprecated behaviour";
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB: return "undefined behaviour";
    case GL_DEBUG_TYPE_PERFORMANCE_ARB: return "performance warning";
    case GL_DEBUG_TYPE_PORTABILITY_ARB: return "portability warning";
    case GL_DEBUG_TYPE_OTHER_ARB: return "other";
    default: return "unknown";
  }
}

std::string ClassifySeverity(GLenum severity) {
  switch(severity) {
    case GL_DEBUG_SEVERITY_HIGH_ARB: return "high";
    case GL_DEBUG_SEVERITY_MEDIUM_ARB: return "medium";
    case GL_DEBUG_SEVERITY_LOW_ARB: return "low";
    case GL_DEBUG_SEVERITY_NOTIFICATION: return "notification";
    default: return "unknown";
  }
}

void GLAPIENTRY MyDebugCallback(GLenum source, GLenum type, GLuint id,
                              GLenum severity, GLsizei length,
                              const GLchar* message, const GLvoid* userParam) {
  const auto src = ClassifySource(source);
  const auto t = ClassifyType(type);
  const auto sev = ClassifySeverity(severity);
  const std::string mes = Str() << "GL " << t << "(" << sev << "): " << src << ": " << message;
  if( severity == GL_DEBUG_SEVERITY_HIGH_ARB) {
    LogError(mes);
    throw mes;
  }
  else {
    LogInfo(mes);
  }
}

bool OglDebug::IsSupported() { return GLEW_ARB_debug_output == GL_TRUE; }

OglDebug::OglDebug(bool d) : debug_enabled_(d) {
  LOGINFO("Starting OpenGL debug");
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
