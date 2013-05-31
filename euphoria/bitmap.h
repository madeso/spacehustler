// Euphoria - Copyright (c) Gustav

#ifndef EUPHORIA_BITMAP_H_
#define EUPHORIA_BITMAP_H_

#include <boost/scoped_array.hpp>
#include <boost/noncopyable.hpp>

class Color;

/** A in-memory representation of a bitmap.
@todo remove useless painting functions
 */
class Bitmap
    : boost::noncopyable {
  public:
    /** The size type of the image.
     */
    typedef unsigned int Size;

    /** The type of a byte.
     */
    typedef unsigned char Byte;

    /** The types of bitmaps this class can represent.
     */
    enum Type {
      /** RGB image.
       */
      Rgb,

      /** RGB image wit a alpha component.
       */
      Rgba
    };

    /** Construct a new Bitmap.
    @param width the width of the image.
    @param height the height of the image.
    @param type the RGB type of the image.
     */
    Bitmap(Size width, Size height, Type type);

    /** Destruct the bitmap.
     */
    ~Bitmap();

    /** Paints a pixel in the image.
    This takes the pixel and changes it depending on the color and alpha of the input and modifying it.
    @param x the x coordinate of the pixel.
    @param y the y coordinate of the pixel.
    @param c the color to fuse with the current pixel.
    @see setPixel
     */
    void paintPixel(Size x, Size y, const Color& c);

    /** Set the color of the pixel in a image.
    This ignores the previous color of the pixel and sets it to the new color.
    @param x the x coordinate of the pixel.
    @param y the y coordinate of the pixel.
    @param c the color of the new pixel.
    @see paintPixel
     */
    void setPixel(Size x, Size y, const Color& c);

    /** Get the color of a single pixel.
    @param x the x coordinate of the pixel.
    @param y the y coordinate of the pixel.
    @return the color of the pixel.
     */
    const Color getPixel(Size x, Size y) const;

    /** Get the width of the image.
    @return the width.
     */
    const Size getWidth() const;

    /** Get the height of the image.
    @return the height.
     */
    const Size getHeight() const;

    /** Get the storage type of the image.
    @return the image type.
     */
    const Type getType() const;

    /** Gets the pixel data of the image.
    @return the pixel data.
     */
    const Byte* getPixels() const;

    /** Gets the pixel data of the image.
    @return the pixel data.
     */
    Byte* getPixels();

  private:
    Size width;
    Size height;
    Type type;
    boost::scoped_array<Byte> pixels;
};

#endif  // EUPHORIA_BITMAP_H_
