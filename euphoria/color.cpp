#include "color.h"


Color::Color(float gray)
  : r(gray), g(gray), b(gray), a(1) {
}

Color::Color(float R, float G, float B)
  : r(R), g(G), b(B), a(1) {
}

Color::Color(float R, float G, float B, float A)
  : r(R), g(G), b(B), a(A) {
}

Color::Color(const Color& c, float A)
  : r(c.r), g(c.g), b(c.b), a(A) {
}
