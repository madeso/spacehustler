#include "mesh.h"
#include <cassert>
#include "shader.h"

void Mesh::addPoint(float x, float y, float z)
{
	assert(this);

	vertices.push_back(x);
	vertices.push_back(y);
	vertices.push_back(z);
}

/////////////////////////

Vao::Vao()
	: object(0)
{
	assert(this);

	glGenVertexArrays(1, &object);
}

Vao::~Vao()
{
	assert(this);
	assert(object != 0);

	glDeleteVertexArrays(1, &object);
}

GLuint Vao::get() const
{
	assert(this);
	assert(object != 0);

	return object;
}

void Vao::bind() const
{
	assert(this);
	assert(object != 0);

	glBindVertexArray(object);
}

void Vao::unbind()
{
	assert(this);
	assert(object != 0);

	glBindVertexArray(0);
}

/////////////////////////

Vbo::Vbo()
	: object(0)
{
	assert(this);

	glGenBuffers(1, &object);
}

Vbo::~Vbo()
{
	assert(this);
	assert(object != 0);

	// deltete
	glDeleteBuffers(1, &object);
	object = 0;
}


GLuint Vbo::get() const
{
	assert(this);
	assert(object != 0);

	return object;
}

void Vbo::bind() const
{
	assert(this);
	assert(object != 0);

	glBindBuffer(GL_ARRAY_BUFFER, object);
}

void Vbo::unbind()
{
	assert(this);
	assert(object != 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/////////////////////////

CompiledMesh::CompiledMesh(const Mesh& mesh, const Program& prog)
{
	vao.bind();
	vbo.bind();

	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*mesh.vertices.size(), &mesh.vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(prog.attrib("vert"));
	glVertexAttribPointer(prog.attrib("vert"), 3, GL_FLOAT, GL_FALSE, 0, NULL);

	vbo.unbind();
	vao.unbind();
}

CompiledMesh::~CompiledMesh()
{
}

void CompiledMesh::render()
{
	vao.bind();
	glDrawArrays(GL_TRIANGLES, 0, 3);
	vao.unbind();
}