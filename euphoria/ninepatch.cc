// Euphoria - Copyright (c) Gustav

#include <cassert>
#include <stdexcept>
#include <fstream>  // NOLINT for loading data

#include "euphoria/str.h"
#include "euphoria/ninepatch.h"
#include "euphoria/ogldebug.h"
#include "euphoria/stringutils.h"
#include "euphoria/path.h"
#include "json/json.h"

#include "euphoria/tweak.h"

namespace euphoria {

Ninepatch::Ninepatch(const std::string& texture, float scale)
    : texture_(texture), scale_(scale) {
  assert(this);
}

Ninepatch::~Ninepatch() { assert(this); }

const std::string& Ninepatch::texture() const {
  assert(this);
  return texture_;
}

float Ninepatch::scale() const {
  assert(this);
  return scale_;
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
              std::shared_ptr<Texture> texture, unsigned int face) {
  assert(mesh);
  assert(texture);
  const float iw = texture->width();
  const float ih = texture->height();
  const float x = patch.x / iw;
  const float y = patch.y / ih;
  const float w = patch.width / iw;
  const float h = patch.height / ih;

  const unsigned int base = face * 4;

  const float le = x;
  const float ri = x + w;
  const float up = y;
  const float dn = y + h;

  const float z = 0;

  mesh->AddPoint(le, dn, z, le, up);
  mesh->AddPoint(ri, dn, z, ri, up);
  mesh->AddPoint(ri, up, z, ri, dn);
  mesh->AddPoint(le, up, z, le, dn);
  mesh->AddFace(base + 2, base + 1, base + 0);
  mesh->AddFace(base + 3, base + 2, base + 0);
}

internal::MeshPart CreateNinePatchMesh(const Ninepatch& ninepatch,
                                       std::shared_ptr<Texture> texture) {
  internal::MeshPart ret;
  for (unsigned int i = 0; i < 9; ++i) {
    AddPatch(&ret, ninepatch.GetPatchAt(i), texture, i);
  }
  return ret;
}
}  // namespace

NinepatchInstance::NinepatchInstance(const Ninepatch& ninepatch,
                                     std::shared_ptr<Program> program,
                                     std::shared_ptr<Texture> texture)
    : texture_(texture),
      program_(program),
      width_left_(ninepatch.GetPatchAt(0).width * ninepatch.scale()),
      width_right_(ninepatch.GetPatchAt(2).width * ninepatch.scale()),
      height_up_(ninepatch.GetPatchAt(0).height * ninepatch.scale()),
      height_down_(ninepatch.GetPatchAt(6).height * ninepatch.scale()),
      mesh_(CreateNinePatchMesh(ninepatch, texture), program, texture) {
  assert(this);
  assert(height_up_ > 0);
  assert(height_down_ > 0);
  assert(width_left_ > 0);
  assert(width_right_ > 0);
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

  // 0,0 is in upper left, positive direction right and up
  const float xa = 0;
  const float ya = 0;
  const float xb = width_left_;
  const float yb = -height_up_;
  const float xc = width_left_ + middle_width;
  const float yc = -(height_up_ + middle_height);
  const float xd = size_[0];
  const float yd = -size_[1];

  const float z = 1;

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
  mesh_.SetVertex(0, v0);
  mesh_.SetVertex(1, v1);
  mesh_.SetVertex(2, v5);
  mesh_.SetVertex(3, v4);

  // upper middle
  mesh_.SetVertex(4, v1);
  mesh_.SetVertex(5, v2);
  mesh_.SetVertex(6, v6);
  mesh_.SetVertex(7, v5);

  // upper right
  mesh_.SetVertex(8, v2);
  mesh_.SetVertex(9, v3);
  mesh_.SetVertex(10, v7);
  mesh_.SetVertex(11, v6);

  // middle left
  mesh_.SetVertex(12, v4);
  mesh_.SetVertex(13, v5);
  mesh_.SetVertex(14, v9);
  mesh_.SetVertex(15, v8);

  // middle middle
  mesh_.SetVertex(16, v5);
  mesh_.SetVertex(17, v6);
  mesh_.SetVertex(18, v10);
  mesh_.SetVertex(19, v9);

  // middle right
  mesh_.SetVertex(20, v6);
  mesh_.SetVertex(21, v7);
  mesh_.SetVertex(22, v11);
  mesh_.SetVertex(23, v10);

  // lower left
  mesh_.SetVertex(24, v8);
  mesh_.SetVertex(25, v9);
  mesh_.SetVertex(26, v13);
  mesh_.SetVertex(27, v12);

  // lower middle
  mesh_.SetVertex(28, v9);
  mesh_.SetVertex(29, v10);
  mesh_.SetVertex(30, v14);
  mesh_.SetVertex(31, v13);

  // lower right
  mesh_.SetVertex(32, v10);
  mesh_.SetVertex(33, v11);
  mesh_.SetVertex(34, v15);
  mesh_.SetVertex(35, v14);

  // update mesh
  mesh_.UpdateMesh();
}

void NinepatchInstance::Render(const Camera& camera) {
  assert(this);

  program_->Bind();
  program_->SetUniform("camera", camera.view());
  program_->SetUniform("projection", camera.projection());
  Vec3 p3(position_, 0);
  TWEAK(p3);
  program_->SetUniform("model", CreateMat44(p3));
  texture_->Bind(0);

  glClear(GL_DEPTH_BUFFER_BIT);
  mesh_.Render();
  program_->Unbind();
  OglDebug::Verify();
}

Ninepatch LoadNinepatch(const std::string& filename) {
  std::ifstream in(filename.c_str());
  if (!in.good()) {
    throw std::logic_error(Str() << "Unable to load ui table from  "
                                 << filename);
  }
  Json::Value root;
  Json::Reader reader;
  if (false == reader.parse(in, root)) {
    throw std::logic_error(Str() << "Unable to parse " << filename << ": "
                                 << reader.getFormattedErrorMessages());
  }

  Path pfilename(filename);

  const std::string texturefile =
      root.get("texture", pfilename.ChangeExtension(".png").GetFile())
          .asString();
  const std::string& texture =
      pfilename.GetDirectory().File(texturefile).GetOsPath();
  const float scale = root.get("scale", 1.0f).asFloat();
  Ninepatch ret(texture, scale);
  const std::string names[9] = {"ul", "um", "ur", "ml", "mm",
                                "mr", "ll", "lm", "lr"};
  for (int i = 0; i < 9; ++i) {
    Patch p;
    Json::Value v = root[names[i]];
    p.x = v.get("x", 0).asInt();
    p.y = v.get("y", 0).asInt();
    p.width = v.get("w", 0).asInt();
    p.height = v.get("h", 0).asInt();
    ret.SetPatchAt(i, p);
  }
  return ret;
}

std::shared_ptr<NinepatchInstance> CreateNinepatchInstance(
    const Ninepatch& ninepatch, ShaderCache* shadercache,
    TextureCache* texturecache, const Settings& settings) {
  auto shader = shadercache->GetOrCreate("default.js", settings);
  auto texture = texturecache->GetOrCreate(
      TextureLoadingInstruction(ninepatch.texture(), WrapMode::CLAMP_TO_EDGE,
                                WrapMode::CLAMP_TO_EDGE,
                                ImageStorage::COMPRESSED_RGBA),
      settings);
  std::shared_ptr<NinepatchInstance> ret(
      new NinepatchInstance(ninepatch, shader, texture));
  return ret;
}

}  // namespace euphoria
