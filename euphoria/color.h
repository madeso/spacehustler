// Euphoria - Copyright (c) Gustav

#ifndef EUPHORIA_COLOR_H_
#define EUPHORIA_COLOR_H_

class Color {
  public:
    explicit Color(float gray);
    Color(float R, float G, float B);
    Color(float R, float G, float B, float A);
    Color(const Color& c, float A);

    float r, g, b, a;
};

#endif  // EUPHORIA_COLOR_H_
