// Euphoria - Copyright (c) Gustav

#include "euphoria/texture.h"

#include <cassert>
#include <string>

#include "soil/SOIL.h"
#include "euphoria/str.h"

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
    case WrapMode::kWrap_Repeat:
      return GL_REPEAT;
    case WrapMode::kWrap_MirrorRepeat:
      return GL_MIRRORED_REPEAT;
    case WrapMode::kWrap_ClampToEdge:
      return GL_CLAMP_TO_EDGE;
    default:
      const std::string error = Str() << "Unknown texture wrap mode";
      throw error;
  }
}

GLint C(FilterMode mode) {
  switch (mode) {
    case FilterMode::kFilter_Linear:
      return GL_LINEAR;
    case FilterMode::kFilter_Nearest:
      return GL_NEAREST;
    case FilterMode::kFilter_Mimap:
      return GL_LINEAR_MIPMAP_LINEAR;
    default:
      const std::string error = Str() << "Unknown texture filter mode";
      throw error;
  }
}

GLint C(TextureType type) {
  switch (type) {
    case TextureType::kType_Rgb:
      return GL_RGB;
    case TextureType::kType_Rgba:
      return GL_RGBA;
    case TextureType::kType_CompressedRgb:
      return GL_COMPRESSED_RGB;
    case TextureType::kType_CompressedRgba:
      return GL_COMPRESSED_RGBA;
    default:
      const std::string error = Str() << "Unknown texture type";
      throw error;
  }
}
}  // namespace

Texture::Texture(const ImageData& data, TextureType textureType, WrapMode wraps,
                 WrapMode wrapt, FilterMode filter, float anisotropic) {
  assert(this);
  glBindTexture(GL_TEXTURE_2D, texture_);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, C(filter));
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, C(filter));
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, C(wraps));
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, C(wrapt));
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropic);

  glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP,
                  filter == FilterMode::kFilter_Mimap ? GL_TRUE : GL_FALSE);
  glTexImage2D(GL_TEXTURE_2D, 0, C(textureType), (GLsizei)data.width(),
               (GLsizei)data.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
               data.pixels());

  if (textureType == TextureType::kType_CompressedRgb ||
      textureType == TextureType::kType_CompressedRgba) {
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
