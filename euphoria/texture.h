  // Euphoria - Copyright (c) Gustav
// Euphoria is awesome: 幸福感是真棒

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
      GLuint object() const;

      /** Utility function for getting the OpenGL texture object handle.
      @see get()
      @return the texture object handle.
       */
      operator GLuint() const;
    private:
      GLuint object_;
  };
}  // namespace internal

/** Get the maximum level of anistrop this computer supports.
More info can be found on wikipedia:
http://en.wikipedia.org/wiki/Anistropic_filtering
@returns the anistropy level.
 */
float GetMaxAnistropy();

/** A general open gl image
 */
class Image
    : boost::noncopyable {
  public:
    /** Constructor.
    @param alpha true if it should contain alpha, false if not
    @param width the width of the image
    @param height the height of the image
    @param bitmapData the bitmap data to use
    @param mipmap true if to mipmap, false if not
    @param format the image format
    @param anistropy the anistropy level
    @param compress true if image should be compressed, false if not
     */
    Image(bool alpha, int width, int height, const char* bitmapData,
          bool mipmap, int format, float anistropy, bool compress);
    ~Image();

    /** Bind image to a position.
    @param position the position
     */
    void Bind(int position) const;

    /** Get the texture id.
     */
    unsigned int texture() const;
  private:
    unsigned int texture_;
};

/** A OpenGL texture.
@todo merge with Image
 */
class Texture {
  public:
    /** Type defining how to wrap the texture.
     */
    enum WrapMode {
      /** Repeat it.
       */
      kWrap_Repeat,

      /** Repeat, but mirror instead of restarting.
       */
      kWrap_MirrorRepeat,

      /** Clamp to the edge.
       */
      kWrap_ClampToEdge
    };

    /** The texture filtering mode.
     */
    enum FilterMode {
      /** Nearest filtering. Good for images that doesn't look good when scaled.
       */
      kFilter_Nearest,

      /** Linear filtering. Good for textures.
       */
      kFilter_Linear
    };

    /** How the image is stored.
     */
    enum Type {
      /** Store it as RGB without alpha.
       */
      kType_Rgb,

      /** Store is a RGB with alpha.
       */
      kType_Rgba,

      /** Store it as compressed RGB without alpha.
       */
      kType_CompressedRgb,

      /** Store it as compressed RGB with alpha.
       */
      kType_CompressedRgba
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
    void Bind(unsigned int index) const;

  private:
    internal::TextureObject texture_;
};

#endif  // EUPHORIA_TEXTURE_H_

