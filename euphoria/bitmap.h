// Euphoria - Copyright (c) Gustav

#ifndef EUPHORIA_BITMAP_H_
#define EUPHORIA_BITMAP_H_

#include <boost/scoped_array.hpp>
#include <boost/noncopyable.hpp>

#include <string>

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

    /** Load a bitmap from file.
    @param path the path to the bitmap.
     */
    explicit Bitmap(const std::string& path);

    /** Destruct the bitmap.
     */
    ~Bitmap();

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
