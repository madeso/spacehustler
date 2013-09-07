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

/** Get the maximum level of anisotropic this computer supports.
More info can be found on wikipedia:
http://en.wikipedia.org/wiki/Anistropic_filtering
@returns the anisotropic level.
 */
float GetMaxAnistropy();

/** Represents texture data, not sent to open gl.
 */
class ImageData : boost::noncopyable {
  public:
    /** Load image from file.
    @param path the path of the image to use
     */
    explicit ImageData(const std::string& path);

    /** Create a null image.
    @param width the width of the image
    @param height the height of the image
    @param channels the number of channels
     */
    ImageData(int width, int height, int channels);

    /** Destructor.
     */
    ~ImageData();

    /** Gets the width of the image.
    @returns the width of the image
     */
    int width() const;

    /** Gets the height of the image.
    @returns the height of the image
     */
    int height() const;

    /** Gets the channels.
    @returns the channels
     */
    int channels() const;

    /** Gets the pixel data.
    @returns the pixel data
     */
    unsigned char* pixels() const;

  private:
    int width_;
    int height_;
    int channels_;
    unsigned char* pixels_;
};

/** A OpenGL texture.
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
      kFilter_Linear,

      /** Mipmap linear filtering, good for textures.
      */
      kFilter_Mimap
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
    @param data the image data
    @param textureType how to store the texture
    @param wraps Wrapping type for S UV coordinate
    @param wrapt Wrapping type for T UV coordinate
    @param filter how to filter the texture when rendering
    @param anisotropic the anisotropic data
     */
    Texture(const ImageData& data, Type textureType, WrapMode wraps
            , Texture::WrapMode wrapt, FilterMode filter, float anisotropic);

    /** Destructs the texture.
     */
    ~Texture();

    /** Binds the current texture.
    @param index bind to this index.
     */
    void Bind(unsigned int index) const;

    /** Get the texture.
    @returns the texture
     */
    const internal::TextureObject& texture() const;

  private:
    internal::TextureObject texture_;
};

#endif  // EUPHORIA_TEXTURE_H_

