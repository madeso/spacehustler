// Euphoria - Copyright (c) Gustav

#include "euphoria/mesh.h"
#include <cassert>
#include "euphoria/shader.h"

Mesh::Mesh()
  : points(0) {
}

void Mesh::addPoint(GLfloat x, GLfloat y, GLfloat z, GLfloat u, GLfloat v) {
  assert(this);

  vertices.push_back(x);
  vertices.push_back(y);
  vertices.push_back(z);

  vertices.push_back(u);
  vertices.push_back(v);

  ++points;
}

/////////////////////////

Vao::Vao()
  : object(0) {
  assert(this);

  glGenVertexArrays(1, &object);
}

Vao::~Vao() {
  assert(this);
  assert(object != 0);

  glDeleteVertexArrays(1, &object);
}

GLuint Vao::get() const {
  assert(this);
  assert(object != 0);

  return object;
}

void Vao::bind() const {
  assert(this);
  assert(object != 0);

  glBindVertexArray(object);
}

void Vao::unbind() {
  assert(this);
  assert(object != 0);

  glBindVertexArray(0);
}

/////////////////////////

Vbo::Vbo()
  : object(0) {
  assert(this);

  glGenBuffers(1, &object);
}

Vbo::~Vbo() {
  assert(this);
  assert(object != 0);

  // delete
  glDeleteBuffers(1, &object);
  object = 0;
}


GLuint Vbo::get() const {
  assert(this);
  assert(object != 0);

  return object;
}

void Vbo::bind() const {
  assert(this);
  assert(object != 0);

  glBindBuffer(GL_ARRAY_BUFFER, object);
}

void Vbo::unbind() {
  assert(this);
  assert(object != 0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/////////////////////////

CompiledMesh::CompiledMesh(const Mesh& mesh, boost::shared_ptr<Program> prog,
                           boost::shared_ptr<Texture> tex)
  : program(prog)
  , texture(tex)
  , points(mesh.points) {
  vao.bind();
  vbo.bind();

  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*mesh.vertices.size(),
               &mesh.vertices[0], GL_STATIC_DRAW);

  const GLsizei stride = 5 * sizeof(GLfloat);
  const GLvoid* uvoffset = reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat));

  glEnableVertexAttribArray(program->attrib("vert"));
  glVertexAttribPointer(program->attrib("vert"), 3, GL_FLOAT, GL_FALSE, stride
                        , NULL);

  glEnableVertexAttribArray(program->attrib("vertuv"));
  glVertexAttribPointer(program->attrib("vertuv"), 2, GL_FLOAT, GL_TRUE, stride,
                        uvoffset);

  vbo.unbind();
  vao.unbind();
}

CompiledMesh::~CompiledMesh() {
}

void CompiledMesh::render(const Camera& camera, const mat44& model) {
  program->bind();
  program->setUniform("camera", camera.view);
  program->setUniform("projection", camera.projection);
  program->setUniform("model", model);
  texture->bind(0);
  program->setUniform("tex", 0);
  vao.bind();
  glDrawArrays(GL_TRIANGLES, 0, points);
  vao.unbind();

  program->unbind();
}
