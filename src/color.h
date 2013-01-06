#ifndef COLOR_H
#define COLOR_H

class Color
{
public:
	Color(float gray);
	Color(float R, float G, float B);
	Color(float R, float G, float B, float A);
	Color(const Color& c, float A);

	float r, g, b, a;
};

#endif