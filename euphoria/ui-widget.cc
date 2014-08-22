// Euphoria - Copyright (c) Gustav

#include "euphoria/ui-widget.h"

#include <cassert>

namespace euphoria {
namespace ui {

Widget::Widget() { assert(this); }
Widget::~Widget() { assert(this); }

Vec2 Widget::position() const {
  assert(this);
  return position_;
}
void Widget::set_position(const Vec2& position) {
  assert(this);
  position_ = position;
}

Vec2 Widget::size() const {
  assert(this);
  return size_;
}
void Widget::set_size(const Vec2& size) {
  assert(this);
  assert(size[0] >= 0);
  assert(size[1] >= 0);
  size_ = size;
}

}  // namespace ui
}  // namespace euphoria
