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

}  // namespace ui
}  // namespace euphoria
