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
  // in pixels
  PIXEL,

  // in percent of size
  RELATIVE,

  // in percentage of the rest of the available area
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

const Vec2 GetSize(SizeRule rule, Vec2 size, Vec2 available);

}  // namespace ui
}  // namespace euphoria

#endif  // EUPHORIA_UI_ENUMS_H_
