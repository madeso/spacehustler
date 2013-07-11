// Euphoria - Copyright (c) Gustav

/** @file
Logic for the textures.
 */

#ifndef EUPHORIA_TEXTURE_H_
#define EUPHORIA_TEXTURE_H_

#include <boost/noncopyable.hpp>

#include <string>

#include "euphoria/opengl.h"

namespace internal {
  /** A OpenGL texture object.
   */
  class TextureObject
      : boost::noncopyable {
    public:
      /** Constructs a new texture object.
       */
      TextureObject();

      /** Destructs the texture object.
       */
      ~TextureObject();

      /** Get the OpenGL texture object handle.
      @return the texture object handle.
       */
      GLuint get() const;

      /** Utility function for getting the OpenGL texture object handle.
      @see get()
      @return the texture object handle.
       */
      operator GLuint() const;
    private:
      GLuint object;
  };
}  // namespace internal

class Bitmap;

/** A OpenGL texture.
 */
class Texture {
  public:
    /** Type defining how to wrap the texture.
     */
    enum WrapMode {
      /** Repeat it.
       */
      Wrap_Repeat,

      /** Repeat, but mirror instead of restarting.
       */
      Wrap_MirrorRepeat,

      /** Clamp to the edge.
       */
      Wrap_ClampToEdge
    };

    /** The texture filtering mode.
     */
    enum FilterMode {
      /** Nearest filtering. Good for images that doesn't look good when scaled.
       */
      Filter_Nearest,

      /** Linear filtering. Good for textures.
       */
      Filter_Linear
    };

    /** How the image is stored.
     */
    enum Type {
      /** Store it as RGB without alpha.
       */
      Type_Rgb,

      /** Store is a RGB with alpha.
       */
      Type_Rgba,

      /** Store it as compressed RGB without alpha.
       */
      Type_CompressedRgb,

      /** Store it as compressed RGB with alpha.
       */
      Type_CompressedRgba
    };

    /** Construct a new texture object.
    @param path the path of the image to use.
    @param textureType how to store the texture.
    @param wraps Wrapping type for S UV coordinate.
    @param wrapt Wrapping type for T UV coordinate.
    @param filter how to filter the texture when rendering.
     */
    Texture(const std::string& path, Type textureType, WrapMode wraps
            , Texture::WrapMode wrapt, FilterMode filter);

    /** Destructs the texture.
     */
    ~Texture();

    /** Binds the current texture.
    @param index bind to this index.
     */
    void bind(unsigned int index) const;

  private:
    internal::TextureObject tex;
};

#endif  // EUPHORIA_TEXTURE_H_
