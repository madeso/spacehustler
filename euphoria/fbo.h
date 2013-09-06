  // Euphoria - Copyright (c) Gustav
// Euphoria is awesome: 幸福感是真棒

/** @file
FBO related code.
 */

#ifndef EUPHORIA_FBO_H_
#define EUPHORIA_FBO_H_

#include <boost/noncopyable.hpp>
#include <memory>

class Image;

/** A render buffer.
 */
class RenderBuffer
    : boost::noncopyable {
  public:
    /** Constructor.
    @param internalFormat the intern open gl format
    @param width the width of the buffer
    @param height the height of the buffer
     */
    RenderBuffer(int internalFormat, int width, int height);

    /** Destructor.
     */
    ~RenderBuffer();

    /** Bind this buffer.
     */
    void bind();

    /** Get the buffer id.
    @returns the buffer id
     */
    unsigned int getBuffer() const;
  private:
    unsigned int buffer;
};

/** A frame buffer object.
This is useful when rendering to a texture.
 */
class Fbo
    : boost::noncopyable {
  public:
    /** Constructor.
    @param w the width of the fbo
    @param h the height of the fbo
    @param mipmap true if to mipmap, false if not
     */
    Fbo(int w, int h, bool mipmap);

    /** Binds the rendered texture.
    @param position the position to bind to
     */
    void BindTexture(int position);

    /** The width of the fbo.
    @returns the width
     */
    int width() const;

    /** The height of the fbo.
    @returns the height of the fbo
     */
    int height() const;

    /** Binds this fbo.
     */
    void Bind();
  private:
    unsigned int fbo_;
    int width_;
    int height_;
    std::unique_ptr<RenderBuffer> depth_buffer_;
    std::unique_ptr<Image> texture_;
};

/** Raii for updating a Fbo.
@see Fbo
 */
class TextureUpdator
    : boost::noncopyable {
  public:
    /** Constructor.
    @param fbo the fbo to update.
     */
    explicit TextureUpdator(Fbo* fbo);

    /** Destructor.
     */
    ~TextureUpdator();
};

#endif  // EUPHORIA_FBO_H_
