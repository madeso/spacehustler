// Euphoria - Copyright (c) Gustav

#include "euphoria/mesh.h"
#include <cassert>
#include <vector>
#include <limits>
#include "euphoria/shader.h"

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

namespace internal {
  MeshPart::MeshPart()
    : points(0) {
  }

  void MeshPart::addPoint(GLfloat x, GLfloat y, GLfloat z, GLfloat u,
                          GLfloat v) {
    assert(this);

    vertices.push_back(x);
    vertices.push_back(y);
    vertices.push_back(z);

    vertices.push_back(u);
    vertices.push_back(v);

    ++points;
  }

  void MeshPart::addFace(unsigned int a, unsigned int b, unsigned int c) {
    assert(this);

    assert(IsWithinUnsignedShortRange(a));
    assert(IsWithinUnsignedShortRange(b));
    assert(IsWithinUnsignedShortRange(c));

    faces.push_back(a);
    faces.push_back(b);
    faces.push_back(c);
  }

  Material::Material()
    : wraps(Texture::Wrap_Repeat)
    , wrapt(Texture::Wrap_Repeat) {
  }

}  // namespace internal

/////////////////////////

Mesh::Mesh() {
}

/////////////////////////

namespace internal {
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

  BufferObject::BufferObject()
    : object(0) {
    assert(this);

    glGenBuffers(1, &object);
  }

  BufferObject::~BufferObject() {
    assert(this);
    assert(object != 0);

    // delete
    glDeleteBuffers(1, &object);
    object = 0;
  }


  GLuint BufferObject::get() const {
    assert(this);
    assert(object != 0);

    return object;
  }

  // /////////////////////////

  void ArrayBuffer::bind() const {
    assert(this);

    glBindBuffer(GL_ARRAY_BUFFER, get());
  }

  void ArrayBuffer::unbind() {
    assert(this);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  /////////////////////////

  void ElementArrayBuffer::bind() const {
    assert(this);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, get());
  }

  void ElementArrayBuffer::unbind() {
    assert(this);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }

  /////////////////////////

  CompiledMeshPart::CompiledMeshPart(const MeshPart& mesh,
                                     boost::shared_ptr<Program> prog,
                                     boost::shared_ptr<Texture> tex)
    : program(prog)
    , texture(tex)
    , points(mesh.points)
    , elementCount(mesh.faces.size()) {
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
    glVertexAttribPointer(program->attrib("vertuv"), 2, GL_FLOAT, GL_TRUE,
                          stride, uvoffset);

    elements.bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.faces.size() * sizeof(GLushort),
                 &mesh.faces[0], GL_STATIC_DRAW);
    elements.unbind();

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
    elements.bind();
    const GLvoid* stride = 0;
    glDrawElements(GL_TRIANGLES, elementCount, GL_UNSIGNED_SHORT, stride);
    elements.unbind();
    vao.unbind();

    program->unbind();
  }

  ///////////////////////////

}  // namespace internal

struct Material {
  boost::shared_ptr<Program> program;
  boost::shared_ptr<Texture> texture;
};



/**
@todo add shader + texture cashing
@todo implement better material + shader loading routine.
 */
CompiledMesh::CompiledMesh(const Mesh& mesh, TextureCache* texturecache,
                           ShaderCache* shadercache) {
  std::vector<Material> materials;
  for (auto src : mesh.materials) {
    Material m;
    m.texture = texturecache->get(TextureLoadingInstruction(src.texture,
                                  src.wraps, src.wrapt));
    m.program = shadercache->get("default.js");
    materials.push_back(m);
  }

  for (unsigned int i = 0; i < mesh.parts.size(); ++i) {
    Material m = materials[mesh.parts[i].material];
    boost::shared_ptr<internal::CompiledMeshPart> part(
      new internal::CompiledMeshPart(mesh.parts[i], m.program, m.texture));
    parts.push_back(part);
  }
}

void CompiledMesh::render(const Camera& camera, const mat44& model) {
  for (auto part : parts) {
    part->render(camera, model);
  }
}
