// Euphoria - Copyright (c) Gustav

/** @file
Camera related code.
 */

#ifndef EUPHORIA_NINEPATCH_H_
#define EUPHORIA_NINEPATCH_H_

#include <memory>
#include <string>

#include "euphoria/math.h"
#include "euphoria/mesh.h"

namespace euphoria {
class Texture;
class Camera;

class Patch {
 public:
  Patch();
  Patch(int x, int y, int width, int height);

  int x;
  int y;
  int width;
  int height;

  enum {
    UL,
    UM,
    UR,
    ML,
    MM,
    MR,
    LL,
    LM,
    LR
  };
};

class Ninepatch {
 public:
  Ninepatch(const std::string& texture, float scale);
  ~Ninepatch();

  const std::string& texture() const;

  float scale() const;

  void SetPatchAt(unsigned int index, const Patch& patch);
  const Patch& GetPatchAt(unsigned int index) const;

 private:
  Patch patches_[9];
  const std::string texture_;
  const float scale_;
};

class NinepatchInstance {
 public:
  NinepatchInstance(const Ninepatch& ninepatch,
                    std::shared_ptr<Program> program,
                    std::shared_ptr<Texture> texture);
  ~NinepatchInstance();
  Vec2 position() const;
  void set_position(Vec2 position);

  Vec2 size() const;
  void set_size(Vec2 size);

  float GetMinimumWidth() const;
  float GetMinimumHeight() const;

  void Render(const Camera& camera);

 private:
  void UpdateMesh();

 private:
  std::shared_ptr<Texture> texture_;
  std::shared_ptr<Program> program_;
  Vec2 position_;
  Vec2 size_;
  float width_left_;
  float width_right_;
  float height_up_;
  float height_down_;
  internal::DynamicMeshPart mesh_;
};

Ninepatch LoadNinepatch(const std::string& filename);
std::shared_ptr<NinepatchInstance> CreateNinepatchInstance(
    const Ninepatch& ninepatch, ShaderCache* shadercache,
    TextureCache* texturecache, const Settings& settings);

}  // namespace euphoria

#endif  // EUPHORIA_NINEPATCH_H_
