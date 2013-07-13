// Euphoria - Copyright (c) Gustav

#include "euphoria/color.h"
#include <cassert>

Color::Color(float gray)
  : r(gray), g(gray), b(gray), a(1) {
  assert(this);
}

Color::Color(float R, float G, float B)
  : r(R), g(G), b(B), a(1) {
  assert(this);
}

Color::Color(float R, float G, float B, float A)
  : r(R), g(G), b(B), a(A) {
  assert(this);
}

Color::Color(const Color& c, float A)
  : r(c.r), g(c.g), b(c.b), a(A) {
  assert(this);
}

namespace {
  float C(unsigned int c) {
    return c / 255.0f;
  }
}  // namespace

Color Color::FromHex(unsigned int hex) {
  assert(hex <= 0xFFFFFF);
  const unsigned int red   = (hex & 0xFF0000) >> (4 * 4);
  const unsigned int green = (hex & 0x00FF00) >> (2 * 4);
  const unsigned int blue  = (hex & 0x0000FF) >> 0;
  return Color(C(red), C(green), C(blue));
}
