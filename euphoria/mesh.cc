// Euphoria - Copyright (c) Gustav

#include "euphoria/mesh.h"
#include <cassert>
#include <vector>
#include <limits>
#include "euphoria/shader.h"

namespace euphoria {

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
MeshPart::MeshPart() : points(0), material(0) {}

void MeshPart::AddPoint(GLfloat x, GLfloat y, GLfloat z, GLfloat u, GLfloat v) {
  assert(this);

  vertices.push_back(x);
  vertices.push_back(y);
  vertices.push_back(z);

  vertices.push_back(u);
  vertices.push_back(v);

  ++points;
}

void MeshPart::AddFace(unsigned int a, unsigned int b, unsigned int c) {
  assert(this);

  assert(IsWithinUnsignedShortRange(a));
  assert(IsWithinUnsignedShortRange(b));
  assert(IsWithinUnsignedShortRange(c));

  faces.push_back(a);
  faces.push_back(b);
  faces.push_back(c);
}

Vec3 MeshPart::GetVertex(unsigned int p) const {
  assert(p < points);
  const unsigned int b = p * 5;
  const float x = vertices[b + 0];
  const float y = vertices[b + 1];
  const float z = vertices[b + 2];
  return Vec3(x, y, z);
}

Material::Material() : wraps(WrapMode::REPEAT), wrapt(WrapMode::REPEAT) {}

}  // namespace internal

/////////////////////////

Mesh::Mesh() {}

namespace internal {

/////////////////////////

BasicCompiledMeshPart::BasicCompiledMeshPart(const MeshPart& mesh,
                                             std::shared_ptr<Program> prog,
                                             std::shared_ptr<Texture> tex)
    : program_(prog), texture_(tex), element_count_(mesh.faces.size()) {
  assert(this);
}

BasicCompiledMeshPart::~BasicCompiledMeshPart() { assert(this); }

void BasicCompiledMeshPart::Render() {
  assert(this);
  assert(program_);
  program_->SetUniform("tex", 0);
  vao_.Bind();
  elements_.Bind();
  const GLvoid* stride = 0;
  glDrawElements(GL_TRIANGLES, element_count_, GL_UNSIGNED_SHORT, stride);
  elements_.Unbind();
  vao_.Unbind();
}

void BasicCompiledMeshPart::Render(const Camera& camera, const Mat44& model) {
  assert(this);
  assert(texture_);
  assert(program_);
  program_->Bind();
  /// @todo don't bind everything all the time,
  /// sort and bind only when necessary
  program_->SetUniform("camera", camera.view());
  program_->SetUniform("projection", camera.projection());
  program_->SetUniform("model", model);
  texture_->Bind(0);

  Render();

  program_->Unbind();
}

void BasicCompiledMeshPart::CreateBegin() {
  assert(this);
  vao_.Bind();
  vbo_.Bind();
}

void BasicCompiledMeshPart::CreateVertexes(const std::vector<GLfloat>& vertices,
                                           bool reset) {
  assert(this);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), &vertices[0],
               GL_STATIC_DRAW);
}

void BasicCompiledMeshPart::CreateFaces(const std::vector<GLushort>& faces) {
  assert(this);

  const GLsizei stride = 5 * sizeof(GLfloat);
  const GLvoid* uvoffset = reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat));

  glEnableVertexAttribArray(program_->LookupAttribute("vert"));
  glVertexAttribPointer(program_->LookupAttribute("vert"), 3, GL_FLOAT,
                        GL_FALSE, stride, NULL);

  glEnableVertexAttribArray(program_->LookupAttribute("vertuv"));
  glVertexAttribPointer(program_->LookupAttribute("vertuv"), 2, GL_FLOAT,
                        GL_TRUE, stride, uvoffset);

  elements_.Bind();
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size() * sizeof(GLushort),
               &faces[0], GL_STATIC_DRAW);
  elements_.Unbind();
}

void BasicCompiledMeshPart::CreateEnd() {
  assert(this);
  vbo_.Unbind();
  vao_.Unbind();
}

//////////////////////////////////////////////////////////////////////////

CompiledMeshPart::CompiledMeshPart(const MeshPart& mesh,
                                   std::shared_ptr<Program> program,
                                   std::shared_ptr<Texture> texture)
    : BasicCompiledMeshPart(mesh, program, texture) {
  assert(this);
  CreateBegin();
  CreateVertexes(mesh.vertices, false);
  CreateFaces(mesh.faces);
  CreateEnd();
}

CompiledMeshPart::~CompiledMeshPart() { assert(this); }

//////////////////////////////////////////////////////////////////////////

DynamicMeshPart::DynamicMeshPart(const MeshPart& mesh,
                                 std::shared_ptr<Program> program,
                                 std::shared_ptr<Texture> texture)
    : BasicCompiledMeshPart(mesh, program, texture), vertices_(mesh.vertices) {
  assert(this);

  CreateBegin();
  CreateVertexes(vertices_, false);
  CreateFaces(mesh.faces);
  CreateEnd();
}

DynamicMeshPart::~DynamicMeshPart() { assert(this); }

void DynamicMeshPart::UpdateMesh() {
  assert(this);
  CreateBegin();
  CreateVertexes(vertices_, true);
  CreateEnd();
}

void DynamicMeshPart::SetVertex(unsigned int index, const Vec3 pos, Vec2 uv) {
  assert(this);

  assert(vertices_.size() % 5 == 0);
  assert(index < vertices_.size() / 5);

  const int base_index = index * 5;

  for (int i = 0; i < 3; ++i) {
    vertices_[base_index + i] = pos[i];
  }

  for (int i = 0; i < 2; ++i) {
    vertices_[base_index + 3 + i] = uv[i];
  }
}

//////////////////////////////////////////////////////////////////////////

}  // namespace internal

struct Material {
  std::shared_ptr<Program> program;
  std::shared_ptr<Texture> texture;
};

/**
@todo implement better material + shader loading routine.
 */
CompiledMesh::CompiledMesh(const Mesh& mesh, TextureCache* texturecache,
                           ShaderCache* shadercache, const Settings& settings) {
  std::vector<Material> materials;
  for (auto src : mesh.materials) {
    Material m;
    m.texture = texturecache->GetOrCreate(
        TextureLoadingInstruction(src.texture, src.wraps, src.wrapt), settings);
    m.program = shadercache->GetOrCreate("default.js", settings);
    materials.push_back(m);
  }

  for (unsigned int i = 0; i < mesh.parts.size(); ++i) {
    Material m = materials[mesh.parts[i].material];
    std::shared_ptr<internal::CompiledMeshPart> part(
        new internal::CompiledMeshPart(mesh.parts[i], m.program, m.texture));
    parts_.push_back(part);
  }
}

void CompiledMesh::Render(const Camera& camera, const Mat44& model) {
  for (auto part : parts_) {
    part->Render(camera, model);
  }
}
}  // namespace euphoria
