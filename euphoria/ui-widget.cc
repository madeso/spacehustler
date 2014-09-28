// Euphoria - Copyright (c) Gustav

#include "euphoria/ui-widget.h"

#include <cassert>
#include <utility>

namespace euphoria {
namespace ui {

Widget::Widget()
    : sizerule_(SizeRule::ASPECT_FIT), alignment_(Alignment::CENTER) {
  assert(this);
}
Widget::~Widget() { assert(this); }

SizeRule Widget::sizerule() const {
  assert(this);
  return sizerule_;
}

void Widget::set_sizerule(SizeRule sizerule) {
  assert(this);
  sizerule_ = sizerule;
}

Alignment Widget::alignment() const {
  assert(this);
  return alignment_;
}

void Widget::set_alignment(Alignment alignment) {
  assert(this);
  alignment_ = alignment;
}

std::pair<Vec2, Vec2> GetPositionAndSize(const Vec2 position, const Vec2 size,
                                         SizeRule sizerule, Alignment alignment,
                                         const Vec2& minimumSize) {
  const Vec2 new_size = GetSize(sizerule, Vec2(1, 1), size);
  const Vec2 realtive_position = GetPosition(alignment, new_size, size);

  return std::make_pair(position + realtive_position, new_size);
}

}  // namespace ui
}  // namespace euphoria
