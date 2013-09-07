  // Euphoria - Copyright (c) Gustav
// Euphoria is awesome: 幸福感是真棒

#include "euphoria/texture.h"

#include <cassert>
#include <string>

#include "soil/SOIL.h"

/** @todo change file loading to use a VFS instead.
   */
ImageData::ImageData(const std::string& path)
  : width_(0), height_(0), channels_(0), pixels_(0) {
  assert(this);

  pixels_ = SOIL_load_image(path.c_str(), &width_, &height_,
                            &channels_, SOIL_LOAD_RGBA);

  if (pixels_ == 0) {
    const std::string error = SOIL_last_result();
    throw "Failed to load b/c " + error;
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

namespace {
  typedef unsigned char Byte;
  struct Soil {
    explicit Soil(Byte* pixels)
      : pixels(pixels) {
      if (pixels == 0) {
        const std::string error = SOIL_last_result();
        throw "Failed to load b/c " + error;
      }
    }

    ~Soil() {
      SOIL_free_image_data(pixels);
    }

    Byte* pixels;
  };
}  // namespace

namespace internal {
  TextureObject::TextureObject()
    : object_(0) {
    assert(this);

    glGenTextures(1, &object_);
    if (object_ == 0) {
      throw "Failed to create texture object";
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
  GLfloat anisotropy = 1;
  glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisotropy);

  return anisotropy;
}

namespace {
  GLint C(Texture::WrapMode mode) {
    switch (mode) {
      case Texture::kWrap_Repeat:
        return GL_REPEAT;
      case Texture::kWrap_MirrorRepeat:
        return GL_MIRRORED_REPEAT;
      case Texture::kWrap_ClampToEdge:
        return GL_CLAMP_TO_EDGE;
      default:
        throw "Unknown texture wrap mode";
    }
  }

  GLint C(Texture::FilterMode mode) {
    switch (mode) {
      case Texture::kFilter_Linear:
        return GL_LINEAR;
      case Texture::kFilter_Nearest:
        return GL_NEAREST;
      case Texture::kFilter_Mimap:
        return GL_LINEAR_MIPMAP_LINEAR;
      default:
        throw "Unknown texture filter mode";
    }
  }

  GLint C(Texture::Type type) {
    switch (type) {
      case Texture::kType_Rgb:
        return GL_RGB;
      case Texture::kType_Rgba:
        return GL_RGBA;
      case Texture::kType_CompressedRgb:
        return GL_COMPRESSED_RGB;
      case Texture::kType_CompressedRgba:
        return GL_COMPRESSED_RGBA;
      default:
        throw "Unknown texture type";
    }
  }
}  // namespace

Texture::Texture(const ImageData& data, Type textureType, WrapMode wraps,
                 Texture::WrapMode wrapt, FilterMode filter, float anistropy) {
  assert(this);
  glBindTexture(GL_TEXTURE_2D, texture_);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, C(filter));
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, C(filter));
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, C(wraps));
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, C(wrapt));
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anistropy);

  glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP,
                  filter == kFilter_Mimap ? GL_TRUE : GL_FALSE);
  glTexImage2D(GL_TEXTURE_2D, 0, C(textureType),
               (GLsizei)data.width(), (GLsizei)data.height(), 0,
               GL_RGBA, GL_UNSIGNED_BYTE, data.pixels());

  if (textureType == kType_CompressedRgb
      || textureType == kType_CompressedRgba) {
    GLint result = GL_FALSE;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPRESSED, &result);

    if (result != GL_TRUE) {
      throw "failed to compress image";
    }
  }

  glBindTexture(GL_TEXTURE_2D, 0);  // reset binding
}

Texture::~Texture() {
  assert(this);
}

void Texture::Bind(unsigned int index) const {
  glActiveTexture(GL_TEXTURE0 + index);
  glBindTexture(GL_TEXTURE_2D, texture_);
}

const internal::TextureObject& Texture::texture() const {
  assert(this);
  return texture_;
}
