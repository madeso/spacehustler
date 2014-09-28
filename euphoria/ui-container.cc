// Euphoria - Copyright (c) Gustav

#include "euphoria/ui-container.h"

#include <cassert>

namespace euphoria {
namespace ui {

Container::Container() { assert(this); }
Container::~Container() { assert(this); }

Vec2 Container::position() const {
  assert(this);
  return position_;
}
void Container::set_position(const Vec2& position) {
  assert(this);
  position_ = position;
}

Vec2 Container::size() const {
  assert(this);
  return size_;
}
void Container::set_size(const Vec2& size) {
  assert(this);
  assert(size[0] >= 0);
  assert(size[1] >= 0);
  size_ = size;
}

}  // namespace ui
}  // namespace euphoria
