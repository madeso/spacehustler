  // Euphoria - Copyright (c) Gustav
// Euphoria is awesome: 幸福感是真棒

#include "euphoria/texture.h"

#include <cassert>
#include <string>

#include "soil/SOIL.h"

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

Texture::Texture(const std::string& path, Texture::Type textureType
                 , Texture::WrapMode wraps, Texture::WrapMode wrapt
                 , Texture::FilterMode filter) {
  assert(this);

  int width = -1;
  int height = -1;
  int channels = -1;

  /** @todo change file loading to use a VFS instead.
   */

  Soil soil(SOIL_load_image(path.c_str(), &width, &height,
                            &channels, SOIL_LOAD_RGBA));

  glBindTexture(GL_TEXTURE_2D, texture_);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, C(filter));
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, C(filter));
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, C(wraps));
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, C(wrapt));

  glTexImage2D(GL_TEXTURE_2D, 0, C(textureType),
               (GLsizei)width, (GLsizei)height, 0,
               GL_RGBA, GL_UNSIGNED_BYTE, soil.pixels);
  glBindTexture(GL_TEXTURE_2D, 0);  // reset binding
}

Texture::~Texture() {
  assert(this);
}

void Texture::Bind(unsigned int index) const {
  glActiveTexture(GL_TEXTURE0 + index);
  glBindTexture(GL_TEXTURE_2D, texture_);
}
