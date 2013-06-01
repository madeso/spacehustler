// Euphoria - Copyright (c) Gustav

#include "euphoria/bitmap.h"

#include <cassert>
#include <string>

#include "euphoria/color.h"

#include "soil/SOIL.h"

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
