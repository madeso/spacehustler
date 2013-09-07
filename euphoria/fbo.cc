  // Euphoria - Copyright (c) Gustav
// Euphoria is awesome: 幸福感是真棒

#include "euphoria/fbo.h"

#include <cassert>
#include <string>

#include "euphoria/opengl.h"
#include "euphoria/str.h"
#include "euphoria/texture.h"

RenderBuffer::RenderBuffer(int internalFormat, int width, int height)
  : buffer_(0) {
  glGenRenderbuffers(1, &buffer_);
  Bind();
  glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, width, height);
}

RenderBuffer::~RenderBuffer() {
  glDeleteRenderbuffers(1, &buffer_);
}

void RenderBuffer::Bind() {
  glBindRenderbuffer(GL_RENDERBUFFER, buffer_);
}

unsigned int RenderBuffer::buffer() const {
  return buffer_;
}


void Fbo::Bind() {
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
}

void UnbindFbo() {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Fbo::Fbo(int w, int h, bool mipmap)
  : fbo_(0)
  , width_(w)
  , height_(h) {
  glGenFramebuffers(1, &fbo_);
  Bind();

  depth_buffer_.reset(new RenderBuffer(GL_DEPTH_COMPONENT, width_, height_));
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                            GL_RENDERBUFFER, depth_buffer_->buffer());

  /// @todo investigate if anistropy should be used here to...?
  // for now we just disable it
  const float anistropy = 1.0f;
  ImageData data(width_, height_, 0);
  texture_.reset(new Texture(data,  Texture::kType_Rgba,
                             Texture::kWrap_ClampToEdge,
                             Texture::kWrap_ClampToEdge, mipmap
                             ? Texture::kFilter_Mimap : Texture::kFilter_Linear
                             , anistropy));
  const int mipmaplevel = 0;
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         texture_->texture(), mipmaplevel);

  const int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (status != GL_FRAMEBUFFER_COMPLETE) throw static_cast<std::string>(
      Str() << "Error when creating framebuffer: " << status);

  UnbindFbo();
}

void Fbo::BindTexture(int position) {
  texture_->Bind(position);
}

int Fbo::width() const {
  return width_;
}

int Fbo::height() const {
  return height_;
}

TextureUpdator::TextureUpdator(Fbo* fbo) {
  fbo->Bind();
  glPushAttrib(GL_VIEWPORT_BIT);

  glViewport(0, 0, fbo->width(), fbo->height());
}

TextureUpdator::~TextureUpdator() {
  glPopAttrib();
  UnbindFbo();
}
