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

Padding Widget::padding() const {
  assert(this);
  return padding_;
}
void Widget::set_padding(const Padding& padding) {
  assert(this);
  padding_ = padding;
}

SizeRule Widget::sizerule() const {
  assert(this);
  return sizerule_;
}

void Widget::set_sizerule(SizeRule sizerule){
  assert(this);
  sizerule_ = sizerule;
}

Alignment Widget::alignment() const{
  assert(this);
  return alignment_;
}

void Widget::set_alignment(Alignment alignment){
  assert(this);
  alignment_ = alignment;
}

std::pair<Vec2,Vec2> GetPositionAndSize(const Vec2 position, const Vec2 size, const Padding& padding, SizeRule sizerule, Alignment alignment, const Vec2& minimumSize)
{
  Vec2 modified_position = position;
  Vec2 modified_size = size;
  padding.ModifyWidgetLayout(&modified_position, &modified_size);

  const Vec2 new_size = GetSize(sizerule, Vec2(1, 1), modified_size);
  const Vec2 realtive_position = GetPosition(alignment, new_size, modified_size);

  return std::make_pair(modified_position+realtive_position, new_size);
}

}  // namespace ui
}  // namespace euphoria
