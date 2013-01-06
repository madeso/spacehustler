#include <cassert>

#include "bitmap.h"
#include "color.h"

//#include "SOIL.h"

Bitmap::Bitmap(Bitmap::Size awidth, Bitmap::Size aheight, Bitmap::Type atype)
	: width(awidth), height(aheight), type(atype)
{
	assert(this);
	assert(width > 0);
	assert(height > 0);

	const Size bitsperpixel = atype == Rgba ? 4 : 3;
	pixels.reset(new Bitmap::Byte[ width * height * bitsperpixel ]);
}

Bitmap::~Bitmap()
{
	assert(this);
}

namespace
{
	const Bitmap::Size GetIndex(Bitmap::Size x, Bitmap::Size y, Bitmap::Size Width)
	{
		return x + y * Width;
	}

	float C(Bitmap::Byte c)
	{
		return c / 255.0f;
	}

	Bitmap::Byte C(float f)
	{
		assert(f <= 1);
		assert(f >= 0);
		return static_cast<char>(f * 255);
	}
}

void Bitmap::setPixel(Size x, Size y, const Color& c)
{
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
	if(type == Rgba)
	{
		pixels[index + 3] = C(c.a);
	}
}

const Color Bitmap::getPixel(Size x, Size y) const
{
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

const Bitmap::Size Bitmap::getWidth() const
{
	assert(this);
	assert(width > 0);

	return width;
}

const Bitmap::Size Bitmap::getHeight() const
{
	assert(this);
	assert(height > 0);

	return height;
}

const Bitmap::Type Bitmap::getType() const
{
	assert(this);

	return type;
}

const Bitmap::Byte* Bitmap::getPixels() const
{
	assert(this);
	assert(pixels.get());

	return pixels.get();
}

Bitmap::Byte* Bitmap::getPixels()
{
	assert(this);
	assert(pixels.get());

	return pixels.get();
}

/*void Bitmap::save() const
{
	int save_result =  SOIL_save_image("temp.bmp", SOIL_SAVE_TYPE_BMP, width, height, 3, pixels.get());
}*/
