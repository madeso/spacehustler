// Euphoria - Copyright (c) Gustav

#include "euphoria/openglraii.h"
#include <cassert>

namespace internal {
  Vao::Vao()
    : object_(0) {
    assert(this);

    glGenVertexArrays(1, &object_);
  }

  Vao::~Vao() {
    assert(this);
    assert(object_ != 0);

    glDeleteVertexArrays(1, &object_);
  }

  GLuint Vao::object() const {
    assert(this);
    assert(object_ != 0);

    return object_;
  }

  void Vao::Bind() const {
    assert(this);
    assert(object_ != 0);

    glBindVertexArray(object_);
  }

  void Vao::Unbind() {
    assert(this);
    assert(object_ != 0);

    glBindVertexArray(0);
  }

  /////////////////////////

  BufferObject::BufferObject()
    : object_(0) {
    assert(this);

    glGenBuffers(1, &object_);
  }

  BufferObject::~BufferObject() {
    assert(this);
    assert(object_ != 0);

    // delete
    glDeleteBuffers(1, &object_);
    object_ = 0;
  }


  GLuint BufferObject::object() const {
    assert(this);
    assert(object_ != 0);

    return object_;
  }

  // /////////////////////////

  void ArrayBuffer::Bind() const {
    assert(this);

    glBindBuffer(GL_ARRAY_BUFFER, object());
  }

  void ArrayBuffer::Unbind() {
    assert(this);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  /////////////////////////

  void ElementArrayBuffer::Bind() const {
    assert(this);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object());
  }

  void ElementArrayBuffer::Unbind() {
    assert(this);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }
}  // namespace internal
