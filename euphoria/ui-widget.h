// Euphoria - Copyright (c) Gustav

/** @file
Camera related code.
 */

#ifndef EUPHORIA_UI_WIDGET_H_
#define EUPHORIA_UI_WIDGET_H_

#include "euphoria/math.h"

namespace euphoria {

class Camera;

namespace ui {
class Widget {
 public:
  Widget();
  virtual ~Widget();
  virtual void Draw(const Camera& camera) = 0;

  Vec2 position() const;
  void set_position(const Vec2& position);

  Vec2 size() const;
  void set_size(const Vec2& size);

 private:
  Vec2 position_;
  Vec2 size_;
};
}  // namespace ui
}  // namespace euphoria

#endif  // EUPHORIA_UI_WIDGET_H_
