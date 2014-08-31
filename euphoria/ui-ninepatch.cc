// Euphoria - Copyright (c) Gustav

#include <cassert>

#include "euphoria/ui-ninepatch.h"

namespace euphoria {
namespace ui {

Ninepatch::Ninepatch() : minimum_width_(0.0f), minimum_height_(0.0f) {
  assert(this);
}

Ninepatch::~Ninepatch() { assert(this); }

float Ninepatch::minimum_width() const {
  assert(this);
  return minimum_width_;
}

float Ninepatch::minimum_height() const {
  assert(this);
  return minimum_height_;
}

std::shared_ptr<Texture> Ninepatch::texture() {
  assert(this);
  return texture_;
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
  const float ri = x+w;
  const float up = y;
  const float dn = y+h;

  const float z = 0;

  mesh->AddPoint(le, dn, z, le, dn);
  mesh->AddPoint(ri, dn, z, ri, dn);
  mesh->AddPoint(ri, up, z, ri, up);
  mesh->AddPoint(le, up, z, le, up);
  mesh->AddFace(base+2, base+1, base+0);
  mesh->AddFace(base+3, base+2, base+0);
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
    : width_left_(0.0f),
      width_right_(0.0f),
      height_up_(0.0f),
      height_down_(0.0f),
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
}

void NinepatchInstance::Render() {
  assert(this);
  // update mesh
  const float middle_width = size_[0] - GetMinimumWidth();
  const float middle_height = size_[1] - GetMinimumHeight();

  const float z = 0;

  /*
    a        b         c       d
  a 0--------1---------2-------3
    |        |         |       |
    |   ul   |   um    |   ur  |
    |        |         |       |
  b 4--------5---------6-------7
    |        |         |       |
    |  ml    |   mm    |   mr  |
    |        |         |       |
  c 8--------9---------10-----11
    |        |         |       |
    |   ll   |   lm    |   lr  |
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

  const Vec3 v0(xa,ya,z);
  const Vec3 v1(xb,ya,z);
  const Vec3 v2(xc,ya,z);
  const Vec3 v3(xd,ya,z);

  const Vec3 v4(xa,yb,z);
  const Vec3 v5(xb,yb,z);
  const Vec3 v6(xc,yb,z);
  const Vec3 v7(xd,yb,z);

  const Vec3 v8(xa,yc,z);
  const Vec3 v9(xb,yc,z);
  const Vec3 v10(xc,yc,z);
  const Vec3 v11(xd,yc,z);

  const Vec3 v12(xa,yd,z);
  const Vec3 v13(xb,yd,z);
  const Vec3 v14(xc,yd,z);
  const Vec3 v15(xd,yd,z);

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

  // render mesh
}

}  // namespace ui
}  // namespace euphoria
