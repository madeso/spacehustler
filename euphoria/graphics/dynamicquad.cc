// Euphoria - Copyright (c) Gustav

#include <cassert>
#include <stdexcept>
#include <fstream>  // NOLINT for loading data

#include "euphoria/str.h"
#include "euphoria/graphics/dynamicquad.h"
#include "euphoria/graphics/ogldebug.h"

#include "euphoria/tweak.h"

namespace euphoria {

namespace {
internal::MeshPart CreateQuadMesh() {
  internal::MeshPart mesh;

  const float le = 0;
  const float ri = 1;
  const float up = 0;
  const float dn = 1;

  const float z = 0;

  mesh.AddPoint(le, dn, z, le, up);
  mesh.AddPoint(ri, dn, z, ri, up);
  mesh.AddPoint(ri, up, z, ri, dn);
  mesh.AddPoint(le, up, z, le, dn);
  mesh.AddFace(2, 1, 0);
  mesh.AddFace(3, 2, 0);

  return mesh;
}
}  // namespace

DynamicQuad::DynamicQuad(std::shared_ptr<Program> program,
                         std::shared_ptr<Texture> texture)
    : texture_(texture),
      program_(program),
      position_(0,0),
      size_(1,1),
      mesh_(CreateQuadMesh(), program, texture) {
  assert(this);
}

DynamicQuad::~DynamicQuad() { assert(this); }

Vec2 DynamicQuad::position() const {
  assert(this);
  return position_;
}

void DynamicQuad::set_position(Vec2 position) {
  assert(this);
  position_ = position;
}

Vec2 DynamicQuad::size() const {
  assert(this);
  return size_;
}
void DynamicQuad::set_size(Vec2 size) {
  assert(this);
  size_ = size;

  UpdateMesh();
}

void DynamicQuad::UpdateMesh() {
  assert(this);

  /*
    a        b
  a 0--------1
    |        |
    |  ul/0  |
    |        |
  b 2--------3
  */

  // 0,0 is in upper left, positive direction right and up
  const float xa = 0;
  const float ya = 0;
  const float xb = size_[0];
  const float yb = -size_[1];

  const float z = 1;

  const Vec3 v0(xa, ya, z);
  const Vec3 v1(xb, ya, z);
  const Vec3 v2(xa, yb, z);
  const Vec3 v3(xb, yb, z);

  // vertices are ordered counter clockwise
  // left down -> left right -> up right -> up left

  mesh_.SetVertex(0, v0);
  mesh_.SetVertex(1, v1);
  mesh_.SetVertex(2, v3);
  mesh_.SetVertex(3, v2);

  // update mesh
  mesh_.UpdateMesh();
}

void DynamicQuad::Render(const Camera& camera) {
  assert(this);

  program_->Bind();
  program_->SetUniform("camera", camera.view());
  program_->SetUniform("projection", camera.projection());
  Vec3 pp(position_, 0);
  TWEAK(pp,);
  program_->SetUniform("model", CreateMat44(pp));
  texture_->Bind(0);

  glClear(GL_DEPTH_BUFFER_BIT);
  mesh_.Render();
  program_->Unbind();
  OglDebug::Verify();
}

}  // namespace euphoria
