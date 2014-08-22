// Euphoria - Copyright (c) Gustav

/** @file
Camera related code.
 */

#ifndef EUPHORIA_UI_ENUMS_H_
#define EUPHORIA_UI_ENUMS_H_

namespace euphoria {
namespace ui {

enum class Unit {
  PIXEL,
  RELATIVE
};

enum class SizeRule {
  /// don't size the widget at all
  NONE,

  /// size to fit
  FIT,

  // size to aspect
  ASPECT
};

}  // namespace ui
}  // namespace euphoria

#endif  // EUPHORIA_UI_ENUMS_H_
