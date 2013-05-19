#include <string>

#include "ogldebug.h"
#include "opengl.h"

// definitions shamelessly stolen from glew headers
/* <winnt.h> */
#ifndef CALLBACK
#define GLEW_CALLBACK_DEFINED
#  if defined(__MINGW32__) || defined(__CYGWIN__)
#    define CALLBACK __attribute__ ((__stdcall__))
#  elif (defined(_M_MRX000) || defined(_M_IX86) || defined(_M_ALPHA) \
  || defined(_M_PPC)) && !defined(MIDL_PASS)
#    define CALLBACK __stdcall
#  else
#    define CALLBACK
#  endif
#endif

void OglDebug::Verify() {
  const GLenum err = glGetError();
  if (err != GL_NO_ERROR) {
    std::string name = "unknown gl error";

    switch (err) {
#define ERR(x) case x: name = #x
        ERR(GL_INVALID_OPERATION);
        ERR(GL_INVALID_VALUE);
#undef ERR
    };

    throw "gl error occurred";
  }
}

void CALLBACK MyDebugCallback(GLenum source, GLenum type, GLuint id,
                              GLenum severity, GLsizei length,
                              const GLchar* message, GLvoid* userParam) {
  std::string mes = message;
  throw mes;
}

bool OglDebug::IsSupported() {
  return GLEW_ARB_debug_output == GL_TRUE;
}

OglDebug::OglDebug(bool d)
  : debug(d) {
  if (debug) {
    if (IsSupported() == false) {
      throw "No debug supported";
    }
    Verify();
    glDebugMessageCallbackARB(&MyDebugCallback, 0);
    Verify();
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
    Verify();
  }
}

OglDebug::~OglDebug() {
  if (debug) {
    Verify();
    glDebugMessageCallbackARB(0, 0);
    Verify();
    glDisable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
    Verify();
  }
}
