// Euphoria - Copyright (c) Gustav

#include "euphoria/mesh.h"
#include <cassert>
#include <vector>
#include <limits>
#include "euphoria/shader.h"

MeshPart::MeshPart()
  : points(0) {
}

void MeshPart::addPoint(GLfloat x, GLfloat y, GLfloat z, GLfloat u, GLfloat v) {
  assert(this);

  vertices.push_back(x);
  vertices.push_back(y);
  vertices.push_back(z);

  vertices.push_back(u);
  vertices.push_back(v);

  ++points;
}

namespace {
  bool IsWithinUnsignedShortRange(unsigned int v) {
    if (v < std::numeric_limits<GLushort>::min()) {
      return false;
    }
    if (v > std::numeric_limits<GLushort>::max()) {
      return false;
    }
    return true;
  }
}  // namespace

void MeshPart::addFace(unsigned int a, unsigned int b, unsigned int c) {
  assert(this);

  assert(IsWithinUnsignedShortRange(a));
  assert(IsWithinUnsignedShortRange(b));
  assert(IsWithinUnsignedShortRange(c));

  faces.push_back(a);
  faces.push_back(b);
  faces.push_back(c);
}

/////////////////////////

Mesh::Mesh() {
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

ArrayBuffer::ArrayBuffer()
  : object(0) {
  assert(this);

  glGenBuffers(1, &object);
}

ArrayBuffer::~ArrayBuffer() {
  assert(this);
  assert(object != 0);

  // delete
  glDeleteBuffers(1, &object);
  object = 0;
}


GLuint ArrayBuffer::get() const {
  assert(this);
  assert(object != 0);

  return object;
}

void ArrayBuffer::bind() const {
  assert(this);
  assert(object != 0);

  glBindBuffer(GL_ARRAY_BUFFER, object);
}

void ArrayBuffer::unbind() {
  assert(this);
  assert(object != 0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/////////////////////////

CompiledMeshPart::CompiledMeshPart(const MeshPart& mesh,
                                   boost::shared_ptr<Program> prog,
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

CompiledMeshPart::~CompiledMeshPart() {
}

void CompiledMeshPart::render(const Camera& camera, const mat44& model) {
  /// @todo don't bind everything all the time,
  /// sort and bind only when necessary
  program->bind();
  program->setUniform("camera", camera.view);
  program->setUniform("projection", camera.getProjection());
  program->setUniform("model", model);
  texture->bind(0);
  program->setUniform("tex", 0);
  vao.bind();
  glDrawArrays(GL_TRIANGLES, 0, points);
  vao.unbind();

  program->unbind();
}

///////////////////////////

CompiledMesh::CompiledMesh(const Mesh& mesh,
                           boost::shared_ptr<Program> program) {
  std::vector<boost::shared_ptr<Texture>> materials;
  for (auto src : mesh.materials) {
    boost::shared_ptr<Texture> tex(new Texture(src,
                                   Texture::Type_CompressedRgb,
                                   Texture::Wrap_ClampToEdge,
                                   Texture::Filter_Linear));
    materials.push_back(tex);
  }

  for (unsigned int i = 0; i < mesh.parts.size(); ++i) {
    boost::shared_ptr<CompiledMeshPart> part(
      new CompiledMeshPart(mesh.parts[i],
                           program, materials[mesh.parts[i].material]));
    parts.push_back(part);
  }
}

void CompiledMesh::render(const Camera& camera, const mat44& model) {
  for (auto part : parts) {
    part->render(camera, model);
  }
}
