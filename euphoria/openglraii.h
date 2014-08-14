// Euphoria - Copyright (c) Gustav

/** @file
Raii elements for opengl.
 */

#ifndef EUPHORIA_OPENGLRAII_H_
#define EUPHORIA_OPENGLRAII_H_

#include "euphoria/opengl.h"

namespace euphoria {

namespace internal {
/** A RAII class for a OpenGl Vertex Array Object.
 */
class Vao {
 public:
  /** Construct the VAO.
   */
  Vao();

  /** Destruct the VAO.
   */
  ~Vao();

  /** Get the OpenGL VAO object id.
  @return the object id.
   */
  GLuint object() const;

  /** Utility function for binding this VAO.
   */
  void Bind() const;

  /** Utility function for unbinding this VAO.
   */
  void Unbind();

 private:
  GLuint object_;
};

/** A RAII class for a OpenGL Buffer Object.
 */
class BufferObject {
 protected:
  /** Construct the Buffer.
   */
  BufferObject();

 public:
  /** Destruct the Buffer.
   */
  ~BufferObject();

  /** Get the OpenGL Buffer object id.
  @return the object id.
   */
  GLuint object() const;

 private:
  GLuint object_;
};

/** A RAII class for a OpenGL Array Buffer.
 */
class ArrayBuffer : public BufferObject {
 public:
  /** Utility function for binding this Array Buffer.
   */
  void Bind() const;

  /** Utility function for unbinding this Array Buffer.
   */
  void Unbind();
};

/** A RAII class for a OpenGL Element Array Buffer.
 */
class ElementArrayBuffer : public BufferObject {
 public:
  /** Utility function for binding this Element Array Buffer.
   */
  void Bind() const;

  /** Utility function for unbinding this Element Array Buffer.
   */
  void Unbind();
};
}  // namespace internal

}  // namespace euphoria

#endif  // EUPHORIA_OPENGLRAII_H_
