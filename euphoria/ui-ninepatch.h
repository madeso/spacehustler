// Euphoria - Copyright (c) Gustav

/** @file
Camera related code.
 */

#ifndef EUPHORIA_UI_NINEPATCH_H_
#define EUPHORIA_UI_NINEPATCH_H_

#include <memory>

#include "euphoria/math.h"
#include "euphoria/mesh.h"

namespace euphoria {
class Texture;
namespace ui {
struct Patch {
  int x;
  int y;
  int width;
  int height;
};

class Ninepatch {
 public:
  Ninepatch();
  ~Ninepatch();

  float minimum_width() const;
  float minimum_height() const;

  std::shared_ptr<Texture> texture();

  const Patch& GetPatchAt(unsigned int index) const;

 private:
  Patch patches_[9];
  std::shared_ptr<Texture> texture_;
  float minimum_width_;
  float minimum_height_;
};

class NinepatchInstance {
 public:
  NinepatchInstance(Ninepatch* ninepatch, std::shared_ptr<Program> program);
  ~NinepatchInstance();
  Vec2 position() const;
  void set_position(Vec2 position);

  Vec2 size() const;
  void set_size(Vec2 size);

  float GetMinimumWidth() const;
  float GetMinimumHeight() const;

  void Render();

 private:
  Vec2 position_;
  Vec2 size_;
  float width_left_;
  float width_right_;
  float height_up_;
  float height_down_;
  internal::DynamicMeshPart mesh_;
};

}  // namespace ui
}  // namespace euphoria

#endif  // EUPHORIA_UI_NINEPATCH_H_
