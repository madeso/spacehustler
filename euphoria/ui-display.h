// Euphoria - Copyright (c) Gustav

/** @file
Camera related code.
 */

#include <vector>
#include <memory>
#include <string>

#include "euphoria/ui-table.h"

#ifndef EUPHORIA_UI_DISPLAY_H_
#define EUPHORIA_UI_DISPLAY_H_

namespace euphoria {
class TextureCache;
class Settings;

namespace ui {
class Display {
 public:
  Display(const std::string& file, float width, float height,
          TextureCache* tcache, const Settings& settings);
  void Render();

 private:
  Table table_;
};
}  // namespace ui
}  // namespace euphoria

#endif  // EUPHORIA_UI_DISPLAY_H_
