// Euphoria - Copyright (c) Gustav

#include "euphoria/graphics/texture.h"

#include <cassert>
#include <string>

#include "soil/SOIL.h"
#include "euphoria/str.h"

namespace euphoria {

/** @todo change file loading to use a VFS instead.
   */
ImageData::ImageData(const std::string& path)
    : width_(0), height_(0), channels_(0), pixels_(0) {
  assert(this);

  pixels_ = SOIL_load_image(path.c_str(), &width_, &height_, &channels_,
                            SOIL_LOAD_RGBA);

  if (pixels_ == 0) {
    const std::string error = Str() << "Failed to load b/c "
                                    << SOIL_last_result();
    throw error;
  }
}

ImageData::ImageData(int width, int height, int channels)
    : width_(width), height_(height), channels_(channels), pixels_(0) {
  assert(this);
}

ImageData::~ImageData() {
  assert(this);
  if (pixels_) {
    SOIL_free_image_data(pixels_);
  }
}

int ImageData::width() const {
  assert(this);
  return width_;
}

int ImageData::height() const {
  assert(this);
  return height_;
}

int ImageData::channels() const {
  assert(this);
  return channels_;
}

unsigned char* ImageData::pixels() const {
  assert(this);
  return pixels_;
}

namespace internal {
TextureObject::TextureObject() : object_(0) {
  assert(this);

  glGenTextures(1, &object_);
  if (object_ == 0) {
    const std::string error = Str() << "Failed to create texture object";
    throw error;
  }
}

TextureObject::~TextureObject() {
  assert(this);
  assert(object_ > 0);

  glDeleteTextures(1, &object_);
}

GLuint TextureObject::object() const {
  assert(this);
  assert(object_ > 0);

  return object_;
}

TextureObject::operator GLuint() const {
  assert(this);
  assert(object_ > 0);

  return object();
}
}  // namespace internal

float GetMaxAnistropy() {
  GLfloat anisotropy = 0.0f;
  glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisotropy);

  assert(anisotropy >= 1.0f);
  return anisotropy;
}

namespace {
GLint C(WrapMode mode) {
  switch (mode) {
    case WrapMode::REPEAT:
      return GL_REPEAT;
    case WrapMode::MIRROR_REPEAT:
      return GL_MIRRORED_REPEAT;
    case WrapMode::CLAMP_TO_EDGE:
      return GL_CLAMP_TO_EDGE;
    default:
      const std::string error = Str() << "Unknown texture wrap mode";
      throw error;
  }
}

GLint C(TextureFilter mode) {
  switch (mode) {
    case TextureFilter::LINEAR:
      return GL_LINEAR;
    case TextureFilter::NEAREST:
      return GL_NEAREST;
    case TextureFilter::MIPMAP:
      return GL_LINEAR_MIPMAP_LINEAR;
    default:
      const std::string error = Str() << "Unknown texture filter mode";
      throw error;
  }
}

GLint C(ImageStorage type) {
  switch (type) {
    case ImageStorage::RGB:
      return GL_RGB;
    case ImageStorage::RGBA:
      return GL_RGBA;
    case ImageStorage::COMPRESSED_RGB:
      return GL_COMPRESSED_RGB;
    case ImageStorage::COMPRESSED_RGBA:
      return GL_COMPRESSED_RGBA;
    default:
      const std::string error = Str() << "Unknown texture type";
      throw error;
  }
}
}  // namespace

Texture::Texture(const ImageData& data, ImageStorage textureType,
                 WrapMode wraps, WrapMode wrapt, TextureFilter filter,
                 float anisotropic)
    : width_(data.width()), height_(data.height()) {
  assert(this);
  glBindTexture(GL_TEXTURE_2D, texture_);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, C(filter));
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, C(filter));
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, C(wraps));
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, C(wrapt));
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropic);

  glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP,
                  filter == TextureFilter::MIPMAP ? GL_TRUE : GL_FALSE);
  glTexImage2D(GL_TEXTURE_2D, 0, C(textureType), (GLsizei)data.width(),
               (GLsizei)data.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
               data.pixels());

  if (textureType == ImageStorage::COMPRESSED_RGB ||
      textureType == ImageStorage::COMPRESSED_RGBA) {
    GLint result = GL_FALSE;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPRESSED, &result);

    if (result != GL_TRUE) {
      const std::string error = Str() << "failed to compress image";
      throw error;
    }
  }

  glBindTexture(GL_TEXTURE_2D, 0);  // reset binding
}

Texture::~Texture() { assert(this); }

void Texture::Bind(unsigned int index) const {
  assert(this);
  glActiveTexture(GL_TEXTURE0 + index);
  glBindTexture(GL_TEXTURE_2D, texture_);
}

const internal::TextureObject& Texture::texture() const {
  assert(this);
  return texture_;
}

const int Texture::width() const {
  assert(this);
  return width_;
}

const int Texture::height() const {
  assert(this);
  return height_;
}

}  // namespace euphoria
