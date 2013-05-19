#ifndef BITMAP_H
#define BITMAP_H

#include <boost/scoped_array.hpp>
#include <boost/noncopyable.hpp>

class Color;

class Bitmap
    : boost::noncopyable {
  public:
    typedef unsigned int Size;
    typedef unsigned char Byte;

    enum Type {
      Rgb, Rgba
    };

    Bitmap(Size width, Size height, Type type);
    ~Bitmap();

    void paintPixel(Size x, Size y, const Color& c);
    void setPixel(Size x, Size y, const Color& c);
    const Color getPixel(Size x, Size y) const;

    const Size getWidth() const;
    const Size getHeight() const;

    const Type getType() const;

    const Byte* getPixels() const;
    Byte* getPixels();

    // void save() const;
  private:
    Size width;
    Size height;
    Type type;
    boost::scoped_array<Byte> pixels;
};

#endif
