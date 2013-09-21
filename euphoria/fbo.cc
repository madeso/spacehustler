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
  assert(this);
  glGenRenderbuffers(1, &buffer_);
  Bind();
  glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, width, height);
}

RenderBuffer::~RenderBuffer() {
  assert(this);
  glDeleteRenderbuffers(1, &buffer_);
}

void RenderBuffer::Bind() {
  assert(this);
  glBindRenderbuffer(GL_RENDERBUFFER, buffer_);
}

unsigned int RenderBuffer::buffer() const {
  assert(this);
  return buffer_;
}


void Fbo::Bind() {
  assert(this);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
}

void UnbindFbo() {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Fbo::Fbo(int w, int h, bool mipmap)
  : fbo_(0)
  , width_(w)
  , height_(h) {
  assert(this);
  glGenFramebuffers(1, &fbo_);
  Bind();

  /// @todo investigate if anisotropic should be used here to...?
  // for now we just disable it
  const float anisotropic = 1.0f;
  ImageData data(width_, height_, 0);
  texture_.reset(new Texture(data,  Texture::kType_Rgba,
                             Texture::kWrap_ClampToEdge,
                             Texture::kWrap_ClampToEdge,
                             Texture::kFilter_Nearest
                             , anisotropic));

  depth_buffer_.reset(new RenderBuffer(GL_DEPTH_COMPONENT, width_, height_));
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                            GL_RENDERBUFFER, depth_buffer_->buffer());

  const int mipmaplevel = 0;
  glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                       texture_->texture(), mipmaplevel);

  GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
  glDrawBuffers(1, DrawBuffers);

  const int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (status != GL_FRAMEBUFFER_COMPLETE) throw static_cast<std::string>(
      Str() << "Error when creating framebuffer: " << status);

  UnbindFbo();
}

void Fbo::BindTexture(int position) {
  assert(this);
  texture_->Bind(position);
}

int Fbo::width() const {
  assert(this);
  return width_;
}

int Fbo::height() const {
  assert(this);
  return height_;
}

TextureUpdator::TextureUpdator(Fbo* fbo)
  : width_(fbo->width())
  , height_(fbo->height()) {
  assert(fbo);
  fbo->Bind();
  glPushAttrib(GL_VIEWPORT_BIT);
  SetSubRegion(0.0f, 1.0f, 0.0f, 1.0f);
}

bool IsBetween(float start, float x, float end) {
  if (x < start) {
    return false;
  }
  if (x > end) {
    return false;
  }
  return true;
}

void TextureUpdator::SetSubRegion(float startx, float endx, float starty,
                                  float endy) {
  assert(IsBetween(0.0f, startx, 1.0f));
  assert(IsBetween(0.0f, starty, 1.0f));
  assert(IsBetween(0.0f, endx, 1.0f));
  assert(IsBetween(0.0f, endy, 1.0f));
  assert(startx < endx);
  assert(starty < endy);
  const float width_scale = endx - startx;
  const float height_scale = endy - starty;
  assert(width_scale > 0.0f);
  assert(height_scale > 0.0f);
  glViewport(width_ * startx, height_ * starty, width_ * width_scale,
             height_ * height_scale);
}

TextureUpdator::~TextureUpdator() {
  glFlush();
  glPopAttrib();
  UnbindFbo();
}
