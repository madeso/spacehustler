// Euphoria - Copyright (c) Gustav

#include "euphoria/openglraii.h"
#include <cassert>

namespace internal {
  Vao::Vao()
    : object(0) {
    assert(this);

    glGenVertexArrays(1, &object);
  }

  Vao::~Vao() {
    assert(this);
    assert(object != 0);

    glDeleteVertexArrays(1, &object);
  }

  GLuint Vao::get() const {
    assert(this);
    assert(object != 0);

    return object;
  }

  void Vao::bind() const {
    assert(this);
    assert(object != 0);

    glBindVertexArray(object);
  }

  void Vao::unbind() {
    assert(this);
    assert(object != 0);

    glBindVertexArray(0);
  }

  /////////////////////////

  BufferObject::BufferObject()
    : object(0) {
    assert(this);

    glGenBuffers(1, &object);
  }

  BufferObject::~BufferObject() {
    assert(this);
    assert(object != 0);

    // delete
    glDeleteBuffers(1, &object);
    object = 0;
  }


  GLuint BufferObject::get() const {
    assert(this);
    assert(object != 0);

    return object;
  }

  // /////////////////////////

  void ArrayBuffer::bind() const {
    assert(this);

    glBindBuffer(GL_ARRAY_BUFFER, get());
  }

  void ArrayBuffer::unbind() {
    assert(this);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  /////////////////////////

  void ElementArrayBuffer::bind() const {
    assert(this);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, get());
  }

  void ElementArrayBuffer::unbind() {
    assert(this);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }
}  // namespace internal
