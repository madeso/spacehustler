// Euphoria - Copyright (c) Gustav

#include "euphoria/mesh.h"
#include <cassert>
#include <vector>
#include <limits>
#include "euphoria/shader.h"

namespace {
  bool IsWithinUnsignedShortRange(unsigned int v) {
    /* always false:
     if (v < std::numeric_limits<GLushort>::min()) {
      return false;
    } */
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

  vec3 MeshPart::getVertex(unsigned int p) const {
    assert(p < points);
    const unsigned int b = p * 5;
    const float x = vertices[b + 0];
    const float y = vertices[b + 1];
    const float z = vertices[b + 2];
    return vec3(x, y, z);
  }

  Material::Material()
    : wraps(Texture::Wrap_Repeat)
    , wrapt(Texture::Wrap_Repeat) {
  }

}  // namespace internal

/////////////////////////

Mesh::Mesh() {
}

namespace internal {

  /////////////////////////

  CompiledMeshPart::CompiledMeshPart(const MeshPart& mesh,
                                     std::shared_ptr<Program> prog,
                                     std::shared_ptr<Texture> tex)
    : program(prog)
    , texture(tex)
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
  std::shared_ptr<Program> program;
  std::shared_ptr<Texture> texture;
};



/**
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
    std::shared_ptr<internal::CompiledMeshPart> part(
      new internal::CompiledMeshPart(mesh.parts[i], m.program, m.texture));
    parts.push_back(part);
  }
}

void CompiledMesh::render(const Camera& camera, const mat44& model) {
  for (auto part : parts) {
    part->render(camera, model);
  }
}
