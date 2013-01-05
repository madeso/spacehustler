#ifndef MESH_H
#define MESH_H

#include <vector>

#include "opengl.h"
#include "shader.h"

class Mesh
{
public:
	void addPoint(float x, float y, float z);
	std::vector<float> vertices;
};

class Vao
{
public:
	Vao();
	~Vao();

	GLuint get() const;
	void bind() const;
	void unbind();

private:
	GLuint object;
};

class Vbo
{
public:
	Vbo();
	~Vbo();

	GLuint get() const;
	void bind() const;
	void unbind();
private:
	GLuint object;
};

class CompiledMesh
{
public:
	~CompiledMesh();

	CompiledMesh(const Mesh& mesh, const Program& p);
	void render();

private:
	Vao vao;
	Vbo vbo;
};

#endif