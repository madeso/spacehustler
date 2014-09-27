// Euphoria - Copyright (c) Gustav

#include "euphoria/ui-padding.h"

#include <cassert>

namespace euphoria {
namespace ui {

Padding::Padding(Value left, Value right, Value up, Value down)
    : left_(left), right_(right), up_(up), down_(down) {
  assert(this);
}

void Padding::ModifyWidgetLayout(Vec2* pos, Vec2* size) const {
  assert(this);
  assert(pos);
  assert(size);

  const float width = (*size)[0];
  const float height = (*size)[1];

  // @todo add support for percentage!
  assert(left_.IsRestPercentage() == false);
  assert(right_.IsRestPercentage() == false);
  assert(up_.IsRestPercentage() == false);
  assert(down_.IsRestPercentage() == false);

  const float suggested_left = left_.value(width);
  const float suggested_right = right_.value(width);
  const float suggested_up = up_.value(height);
  const float suggested_down = down_.value(height);

  const float new_width = width - (suggested_left + suggested_right);
  const float new_height = height - (suggested_up + suggested_down);

  const float dx = suggested_left;
  const float dy = -suggested_up;

  (*pos)[0] += dx;
  (*pos)[1] += dy;
  (*size)[0] = new_width;
  (*size)[1] = new_height;
}

}  // namespace ui
}  // namespace euphoria
