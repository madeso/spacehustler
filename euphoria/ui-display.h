// Euphoria - Copyright (c) Gustav

/** @file
Camera related code.
 */

#include <vector>
#include <memory>

#include "euphoria/ui-table.h"

#ifndef EUPHORIA_UI_DISPLAY_H_
#define EUPHORIA_UI_DISPLAY_H_

namespace euphoria {
namespace ui {
class Display {
 public:
  void Render();

 private:
  Table table_;
};
}  // namespace ui
}  // namespace euphoria

#endif  // EUPHORIA_UI_DISPLAY_H_
