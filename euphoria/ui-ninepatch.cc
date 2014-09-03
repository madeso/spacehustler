// Euphoria - Copyright (c) Gustav

#include <cassert>

#include "euphoria/ui-ninepatch.h"
#include "euphoria/ogldebug.h"

namespace euphoria {
namespace ui {

Ninepatch::Ninepatch(std::shared_ptr<Texture> texture) : texture_(texture) {
  assert(this);
  assert(texture);
}

Ninepatch::~Ninepatch() { assert(this); }

std::shared_ptr<Texture> Ninepatch::texture() {
  assert(this);
  return texture_;
}

void Ninepatch::SetPatchAt(unsigned int index, const Patch& patch) {
  assert(this);
  patches_[index] = patch;
}

const Patch& Ninepatch::GetPatchAt(unsigned int index) const {
  assert(this);
  return patches_[index];
}

//////////////////////////////////////////////////////////////////////////

namespace {

void AddPatch(internal::MeshPart* mesh, const Patch& patch,
              std::shared_ptr<Texture> texture) {
  assert(mesh);
  assert(texture);
  const float iw = texture->width();
  const float ih = texture->height();
  const float x = patch.x / iw;
  const float y = patch.y / ih;
  const float w = patch.width / iw;
  const float h = patch.height / ih;

  const unsigned int base = mesh->vertices.size();

  const float le = x;
  const float ri = x + w;
  const float up = y;
  const float dn = y + h;

  const float z = 0;

  mesh->AddPoint(le, dn, z, le, dn);
  mesh->AddPoint(ri, dn, z, ri, dn);
  mesh->AddPoint(ri, up, z, ri, up);
  mesh->AddPoint(le, up, z, le, up);
  mesh->AddFace(base + 2, base + 1, base + 0);
  mesh->AddFace(base + 3, base + 2, base + 0);
}

internal::MeshPart CreateNinePatchMesh(Ninepatch* ninepatch) {
  assert(ninepatch);
  internal::MeshPart ret;
  for (unsigned int i = 0; i < 9; ++i) {
    AddPatch(&ret, ninepatch->GetPatchAt(i), ninepatch->texture());
  }
  return ret;
}
}  // namespace

NinepatchInstance::NinepatchInstance(Ninepatch* ninepatch,
                                     std::shared_ptr<Program> program)
    : texture_(ninepatch->texture()),
      program_(program),
      width_left_(ninepatch->GetPatchAt(0).width /
                  static_cast<float>(ninepatch->texture()->width())),
      width_right_(ninepatch->GetPatchAt(2).width /
                   static_cast<float>(ninepatch->texture()->width())),
      height_up_(ninepatch->GetPatchAt(0).height /
                 static_cast<float>(ninepatch->texture()->height())),
      height_down_(ninepatch->GetPatchAt(6).height /
                   static_cast<float>(ninepatch->texture()->height())),
      mesh_(CreateNinePatchMesh(ninepatch), program, ninepatch->texture()) {
  assert(this);
}

NinepatchInstance::~NinepatchInstance() { assert(this); }

Vec2 NinepatchInstance::position() const {
  assert(this);
  return position_;
}

void NinepatchInstance::set_position(Vec2 position) {
  assert(this);
  position_ = position;
}

Vec2 NinepatchInstance::size() const {
  assert(this);
  return size_;
}

float NinepatchInstance::GetMinimumWidth() const {
  assert(this);
  assert(width_left_ > 0);
  assert(width_right_ > 0);
  return width_left_ + width_right_;
}

float NinepatchInstance::GetMinimumHeight() const {
  assert(this);
  assert(height_up_ > 0);
  assert(height_down_ > 0);
  return height_up_ + height_down_;
}

void NinepatchInstance::set_size(Vec2 size) {
  assert(this);
  assert(size[0] >= GetMinimumWidth());
  assert(size[1] >= GetMinimumHeight());
  size_ = size;

  UpdateMesh();
}

void NinepatchInstance::UpdateMesh() {
  assert(this);
  // update mesh
  const float middle_width = size_[0] - GetMinimumWidth();
  const float middle_height = size_[1] - GetMinimumHeight();

  assert(middle_width >= 0);
  assert(middle_height >= 0);

  /*
    a        b         c       d
  a 0--------1---------2-------3
    |        |         |       |
    |  ul/0  |   um/1  |  ur/2 |
    |        |         |       |
  b 4--------5---------6-------7
    |        |         |       |
    |  ml/3  |   mm/4  |  mr/5 |
    |        |         |       |
  c 8--------9---------10-----11
    |        |         |       |
    |  ll/6  |   lm/7  |  lr/8 |
    |        |         |       |
  d 12-------13--------14-----15
  */

  const float xa = 0;
  const float ya = 0;
  const float xb = width_left_;
  const float yb = height_up_;
  const float xc = width_left_ + middle_width;
  const float yc = height_up_ + middle_height;
  const float xd = size_[0];
  const float yd = size_[1];

  const float z = 0;

  const Vec3 v0(xa, ya, z);
  const Vec3 v1(xb, ya, z);
  const Vec3 v2(xc, ya, z);
  const Vec3 v3(xd, ya, z);

  const Vec3 v4(xa, yb, z);
  const Vec3 v5(xb, yb, z);
  const Vec3 v6(xc, yb, z);
  const Vec3 v7(xd, yb, z);

  const Vec3 v8(xa, yc, z);
  const Vec3 v9(xb, yc, z);
  const Vec3 v10(xc, yc, z);
  const Vec3 v11(xd, yc, z);

  const Vec3 v12(xa, yd, z);
  const Vec3 v13(xb, yd, z);
  const Vec3 v14(xc, yd, z);
  const Vec3 v15(xd, yd, z);

  // vertices are ordered counter clockwise
  // left down -> left right -> up right -> up left

  // upper left
  mesh_.SetVertex(0, v4);
  mesh_.SetVertex(1, v5);
  mesh_.SetVertex(2, v1);
  mesh_.SetVertex(3, v0);

  // upper middle
  mesh_.SetVertex(4, v5);
  mesh_.SetVertex(5, v6);
  mesh_.SetVertex(6, v2);
  mesh_.SetVertex(7, v3);

  // upper right
  mesh_.SetVertex(8, v6);
  mesh_.SetVertex(9, v7);
  mesh_.SetVertex(10, v3);
  mesh_.SetVertex(11, v2);

  // middle left
  mesh_.SetVertex(12, v8);
  mesh_.SetVertex(13, v9);
  mesh_.SetVertex(14, v5);
  mesh_.SetVertex(15, v4);

  // middle middle
  mesh_.SetVertex(16, v9);
  mesh_.SetVertex(17, v10);
  mesh_.SetVertex(18, v6);
  mesh_.SetVertex(19, v5);

  // middle right
  mesh_.SetVertex(20, v10);
  mesh_.SetVertex(21, v11);
  mesh_.SetVertex(22, v7);
  mesh_.SetVertex(23, v6);

  // lower left
  mesh_.SetVertex(24, v12);
  mesh_.SetVertex(25, v13);
  mesh_.SetVertex(26, v9);
  mesh_.SetVertex(27, v8);

  // lower middle
  mesh_.SetVertex(28, v13);
  mesh_.SetVertex(29, v14);
  mesh_.SetVertex(30, v10);
  mesh_.SetVertex(31, v9);

  // lower right
  mesh_.SetVertex(32, v14);
  mesh_.SetVertex(33, v15);
  mesh_.SetVertex(34, v11);
  mesh_.SetVertex(35, v10);

  // update mesh
  mesh_.UpdateMesh();
}

void NinepatchInstance::Render() {
  assert(this);

  Camera camera(800, 600);  // @todo improve camera
  auto nv = camera.view();
  cml::matrix_set_translation(nv, CreateZeroedVec3());
  camera.set_view(nv);

  program_->Bind();
  program_->SetUniform("camera", camera.view());
  program_->SetUniform("projection", camera.projection());
  program_->SetUniform("model", CreateIdentityMat44());
  texture_->Bind(0);

  glClear(GL_DEPTH_BUFFER_BIT);
  mesh_.Render();
  program_->Unbind();
  OglDebug::Verify();
}

}  // namespace ui
}  // namespace euphoria
