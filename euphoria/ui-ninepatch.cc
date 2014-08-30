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

  // render mesh
}

}  // namespace ui
}  // namespace euphoria
