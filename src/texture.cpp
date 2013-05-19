#include "texture.h"

#include "bitmap.h"
#include <cassert>

namespace internal {
  TextureObject::TextureObject()
    : object(0) {
    assert(this);

    glGenTextures(1, &object);
    if (object == 0) {
      throw "Failed to create texture object";
    }
  }

  TextureObject::~TextureObject() {
    assert(this);
    assert(object > 0);

    glDeleteTextures(1, &object);
  }

  GLuint TextureObject::get() const {
    assert(this);
    assert(object > 0);

    return object;
  }

  TextureObject::operator GLuint() const {
    assert(this);
    assert(object > 0);

    return get();
  }
}  // namespace internal

namespace {
  GLint C(Texture::WrapMode mode) {
    switch (mode) {
      case Texture::Wrap_Repeat:
        return GL_REPEAT;
      case Texture::Wrap_MirrorRepeat:
        return GL_MIRRORED_REPEAT;
      case Texture::Wrap_ClampToEdge:
        return GL_CLAMP_TO_EDGE;
      default:
        throw "Unknown texture wrap mode";
    }
  }

  GLint C(Texture::FilterMode mode) {
    switch (mode) {
      case Texture::Filter_Linear:
        return GL_LINEAR;
      case Texture::Filter_Nearest:
        return GL_NEAREST;
      default:
        throw "Unknown texture filter mode";
    }
  }

  GLint C(Texture::Type type) {
    switch (type) {
      case Texture::Type_Rgb:
        return GL_RGB;
      case Texture::Type_Rgba:
        return GL_RGBA;
      case Texture::Type_CompressedRgb:
        return GL_COMPRESSED_RGB;
      case Texture::Type_CompressedRgba:
        return GL_COMPRESSED_RGBA;
      default:
        throw "Unknown texture type";
    }
  }

  GLenum C(Bitmap::Type type) {
    switch (type) {
      case Bitmap::Rgb:
        return GL_RGB;
      case Bitmap::Rgba:
        return GL_RGBA;
      default:
        throw "Unknown bitmap type";
    }
  }
}  // namespace

Texture::Texture(const Bitmap& bitmap, Texture::Type textureType
                 , Texture::WrapMode wrap, Texture::FilterMode filter) {
  assert(this);

  glBindTexture(GL_TEXTURE_2D, tex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, C(filter));
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, C(filter));
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, C(wrap));
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, C(wrap));

  glTexImage2D(GL_TEXTURE_2D, 0, C(textureType),
               (GLsizei)bitmap.getWidth(), (GLsizei)bitmap.getHeight(), 0,
               C(bitmap.getType()),  GL_UNSIGNED_BYTE, bitmap.getPixels());
  glBindTexture(GL_TEXTURE_2D, 0);  // reset binding
}

Texture::~Texture() {
  assert(this);
}

void Texture::bind(unsigned int index) const {
  glActiveTexture(GL_TEXTURE0 + index);
  glBindTexture(GL_TEXTURE_2D, tex);
}
