#ifndef SHADER_H
#define SHADER_H

#include <vector>
#include "opengl.h"
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

class Shader
	: boost::noncopyable

{
public:
	enum Type { Vertex, Fragment };
	static boost::shared_ptr<Shader> FromSource(const std::string& source, const Type& type);
	~Shader();

	GLuint get() const;
private:
	Shader(const Type& type);
	void compile(const std::string& source);
	GLuint shader;
};

class ShaderList
{
public:
	ShaderList& operator()(boost::shared_ptr<Shader> shader);

	std::vector<boost::shared_ptr<Shader> > shaders;
};

class Program
	: boost::noncopyable
{
public:
	static boost::shared_ptr<Program> FromShaderList(const ShaderList& shaders);
	~Program();

	GLuint get() const;

	GLint attrib(const std::string& name) const;
	GLint uniform(const std::string& name) const;

	void setUniform(const std::string& name, int i) const;

	void bind() const;
	void unbind() const;
private:

	Program();
	GLuint program;
};

#endif