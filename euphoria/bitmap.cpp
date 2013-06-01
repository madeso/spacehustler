// Euphoria - Copyright (c) Gustav

#include "euphoria/bitmap.h"

#include <cassert>
#include <string>

#include "euphoria/color.h"

#include "soil/SOIL.h"

// #include "SOIL.h"

Bitmap::Bitmap(Bitmap::Size width, Bitmap::Size height, Bitmap::Type type)
  : width(width), height(height), type(type) {
  assert(this);
  assert(width > 0);
  assert(height > 0);

  const Size bitsperpixel = type == Rgba ? 4 : 3;
  pixels.reset(new Bitmap::Byte[ width * height * bitsperpixel ]);
}

namespace {
  struct Soil {
    explicit Soil(Bitmap::Byte* pixels)
      : pixels(pixels) {
      if (pixels == 0) {
        const std::string error = SOIL_last_result();
        throw "Failed to load b/c " + error;
      }
    }

    ~Soil() {
      SOIL_free_image_data(pixels);
    }

    Bitmap::Byte* pixels;
  };
}  // namespace


Bitmap::Bitmap(const std::string& path)
  : width(0), height(0), type(Rgba) {
  int loadedWidth = -1;
  int loadedHeight = -1;
  int channels = -1;

  /** @todo change file loading to use a VFS instead.
   */

  Soil soil(SOIL_load_image(path.c_str(), &loadedWidth, &loadedHeight,
                            &channels, SOIL_LOAD_RGBA));
  width = loadedWidth;
  height = loadedHeight;
  const Size bitsperpixel = type == Rgba ? 4 : 3;
  pixels.reset(new Bitmap::Byte[ width * height * bitsperpixel ]);
  memcpy(pixels.get(), soil.pixels, width * height * bitsperpixel);

  /** @todo investigate loading soil data direction into a texture for speed.
   */
}

Bitmap::~Bitmap() {
  assert(this);
}

namespace {
  const Bitmap::Size GetIndex(Bitmap::Size x, Bitmap::Size y,
                              Bitmap::Size Width) {
    return x + y * Width;
  }

  float C(Bitmap::Byte c) {
    return c / 255.0f;
  }

  Bitmap::Byte C(float f) {
    assert(f <= 1);
    assert(f >= 0);
    return static_cast<char>(f * 255);
  }
}  // namespace

void Bitmap::setPixel(Size x, Size y, const Color& c) {
  assert(this);
  assert(x >= 0);
  assert(y >= 0);
  assert(x < width);
  assert(y < height);

  const Size bitsperpixel = type == Rgba ? 4 : 3;
  Bitmap::Size index = GetIndex(x, y, width) * bitsperpixel;

  pixels[index + 0] = C(c.r);
  pixels[index + 1] = C(c.g);
  pixels[index + 2] = C(c.b);
  if (type == Rgba) {
    pixels[index + 3] = C(c.a);
  }
}

void Bitmap::paintPixel(Size x, Size y, const Color& c) {
  assert(this);
  assert(x >= 0);
  assert(y >= 0);
  assert(x < width);
  assert(y < height);

  assert(c.a >= 0);
  assert(c.a <= 1);

  const Size bitsperpixel = type == Rgba ? 4 : 3;
  Bitmap::Size index = GetIndex(x, y, width) * bitsperpixel;

  const float a = c.a;
  const float a1 = 1 - c.a;

  Bitmap::Byte& r = pixels[index + 0];
  Bitmap::Byte& g = pixels[index + 1];
  Bitmap::Byte& b = pixels[index + 2];

  r = C(c.r * a + C(r) * a1);
  g = C(c.g * a + C(g) * a1);
  b = C(c.b * a + C(b) * a1);

  if (type == Rgba) {
    pixels[index + 3] = 255;
  }
}

const Color Bitmap::getPixel(Size x, Size y) const {
  assert(this);
  assert(x >= 0);
  assert(y >= 0);
  assert(x < width);
  assert(y < height);

  const Size bitsperpixel = type == Rgba ? 4 : 3;
  Bitmap::Size index = GetIndex(x, y, width) * bitsperpixel;

  const float r = C(pixels[index + 0]);
  const float g = C(pixels[index + 1]);
  const float b = C(pixels[index + 2]);
  const float a = type == Rgba ? C(pixels[index + 3]) : 1;

  return Color(r, g, b, a);
}

const Bitmap::Size Bitmap::getWidth() const {
  assert(this);
  assert(width > 0);

  return width;
}

const Bitmap::Size Bitmap::getHeight() const {
  assert(this);
  assert(height > 0);

  return height;
}

const Bitmap::Type Bitmap::getType() const {
  assert(this);

  return type;
}

const Bitmap::Byte* Bitmap::getPixels() const {
  assert(this);
  assert(pixels.get());

  return pixels.get();
}

Bitmap::Byte* Bitmap::getPixels() {
  assert(this);
  assert(pixels.get());

  return pixels.get();
}

/*void Bitmap::save() const
{
	int save_result =  SOIL_save_image("temp.bmp", SOIL_SAVE_TYPE_BMP, width, height, 3, pixels.get());
}*/
