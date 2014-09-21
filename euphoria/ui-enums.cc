// Euphoria - Copyright (c) Gustav

#include "euphoria/ui-enums.h"

#include <cassert>
#include <algorithm>

namespace euphoria {
namespace ui {

namespace {
float GetScaleFactor(Vec2 from, Vec2 to, int index) {
  return to[index] / from[index];
}
}  // namespace

const Vec2 GetSize(SizeRule rule, Vec2 size, Vec2 available) {
  assert(size[0] > 0);
  assert(size[1] > 0);
  assert(size[0] < available[0]);
  assert(size[1] < available[1]);

  if (rule == SizeRule::ASPECT_FIT) {
    const float f = std::min(GetScaleFactor(size, available, 0),
                             GetScaleFactor(size, available, 1));
    return size * f;
  } else if (rule == SizeRule::ASPECT_FILL) {
    const float f = std::max(GetScaleFactor(size, available, 0),
                             GetScaleFactor(size, available, 1));
    return size * f;
  } else if (rule == SizeRule::FILL) {
    return available;
  }
  assert(false);
  return available;
}

const Vec2 GetPosition(Alignment alignment, Vec2 size, Vec2 available) {
  assert(size[0] <= available[0]);
  assert(size[1] <= available[1]);

  const float x = available[0] - size[0];
  const float y = available[1] - size[1];
  assert(x >= 0);
  assert(y >= 0);

  if (alignment == Alignment::UPPER_LEFT) {
    return Vec2(0, 0);
  } else if (alignment == Alignment::LEFT) {
    return Vec2(0, -y / 2);
  } else if (alignment == Alignment::LOWER_LEFT) {
    return Vec2(0, -y);
  } else if (alignment == Alignment::TOP) {
    return Vec2(x / 2, 0);
  } else if (alignment == Alignment::CENTER) {
    return Vec2(x / 2, -y / 2);
  } else if (alignment == Alignment::DOWN) {
    return Vec2(x / 2, -y);
  } else if (alignment == Alignment::UPPER_RIGHT) {
    return Vec2(x, 0);
  } else if (alignment == Alignment::RIGHT) {
    return Vec2(x, -y / 2);
  } else if (alignment == Alignment::LOWER_RIGHT) {
    return Vec2(x, -y);
  } else {
    assert(false);
    return Vec2(0, 0);
  }
}

}  // namespace ui
}  // namespace euphoria
