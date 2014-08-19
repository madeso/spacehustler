// Euphoria - Copyright (c) Gustav

/** @file
Camera related code.
 */

#ifndef EUPHORIA_UI_NINEPATCH_H_
#define EUPHORIA_UI_NINEPATCH_H_

#include <memory>

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
}  // namespace ui
}  // namespace euphoria

#endif  // EUPHORIA_UI_NINEPATCH_H_
