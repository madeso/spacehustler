#ifndef TEXTURE_H
#define TEXTURE_H

#include "opengl.h"
#include <boost/noncopyable.hpp>

namespace internal
{
	class TextureObject
		: boost::noncopyable
	{
	public:
		TextureObject();
		~TextureObject();

		GLuint get() const;
		operator GLuint() const;
	private:
		GLuint object;
	};
}

class Bitmap;

class Texture
{
public:
	enum WrapMode
	{
	   Wrap_Repeat, Wrap_MirrorRepeat, Wrap_ClampToEdge
	};

	enum FilterMode
	{
	   Filter_Nearest, Filter_Linear
	};

	enum Type
	{
	   Type_Rgb, Type_Rgba, Type_CompressedRgb, Type_CompressedRgba
	};

	Texture(const Bitmap& bitmap, Type type, WrapMode wrap, FilterMode filter);
	~Texture();

	void bind(unsigned int index) const;
private:
	internal::TextureObject tex;
};

#endif