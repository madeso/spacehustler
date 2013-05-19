// Euphoria - Copyright (c) Gustav

#ifndef EUPHORIA_TEXTURE_H_
#define EUPHORIA_TEXTURE_H_

#include <boost/noncopyable.hpp>
#include "euphoria/opengl.h"

namespace internal {
  class TextureObject
      : boost::noncopyable {
    public:
      TextureObject();
      ~TextureObject();

      GLuint get() const;
      operator GLuint() const;
    private:
      GLuint object;
  };
}  // namespace internal

class Bitmap;

class Texture {
  public:
    enum WrapMode {
      Wrap_Repeat, Wrap_MirrorRepeat, Wrap_ClampToEdge
    };

    enum FilterMode {
      Filter_Nearest, Filter_Linear
    };

    enum Type {
      Type_Rgb, Type_Rgba, Type_CompressedRgb, Type_CompressedRgba
    };

    Texture(const Bitmap& bitmap, Type type, WrapMode wrap
            , FilterMode filter);
    ~Texture();

    void bind(unsigned int index) const;
  private:
    internal::TextureObject tex;
};

#endif  // EUPHORIA_TEXTURE_H_
