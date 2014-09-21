// Euphoria - Copyright (c) Gustav

/** @file
Camera related code.
 */

#ifndef EUPHORIA_UI_ENUMS_H_
#define EUPHORIA_UI_ENUMS_H_

#include "euphoria/math.h"

namespace euphoria {
namespace ui {

enum class Unit {
  /// in pixels
  PIXEL,

  /// in percent of size
  RELATIVE,

  /// in percentage of the rest of the available area
  AVAILABLE
};

enum class SizeRule {
  /// don't size the widget at all
  NONE,

  /// size to fill
  FILL,

  // size to aspect, fill
  ASPECT_FILL,

  // size to aspect, fit
  ASPECT_FIT
};

enum class Alignment {
  UPPER_LEFT,
  LEFT,
  LOWER_LEFT,
  TOP,
  CENTER,
  DOWN,
  UPPER_RIGHT,
  RIGHT,
  LOWER_RIGHT
};

const Vec2 GetSize(SizeRule rule, Vec2 size, Vec2 available);

// returns offset from top left, positive is right & up
const Vec2 GetPosition(Alignment alignment, Vec2 size, Vec2 available);

}  // namespace ui
}  // namespace euphoria

#endif  // EUPHORIA_UI_ENUMS_H_
