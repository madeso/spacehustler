// Euphoria - Copyright (c) Gustav

/** @file
Camera related code.
 */

#ifndef EUPHORIA_UI_NINEPATCH_H_
#define EUPHORIA_UI_NINEPATCH_H_

#include <memory>

#include "euphoria/math.h"

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

 private:
  Patch patches_[9];
  std::shared_ptr<Texture> texture_;
  float minimum_width_;
  float minimum_height_;
};

class NinepatchInstance {
public:
  NinepatchInstance();
  ~NinepatchInstance();
  Vec2 position() const;
  void set_position(Vec2 position);

  Vec2 size() const;
  void set_size(Vec2 size);
private:
  Vec2 position_;
  Vec2 size_;
  // DynamicMeshrepresenting the ninepatch
};

}  // namespace ui
}  // namespace euphoria

#endif  // EUPHORIA_UI_NINEPATCH_H_
