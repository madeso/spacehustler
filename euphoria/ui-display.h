// Euphoria - Copyright (c) Gustav

/** @file
Camera related code.
 */

#include <vector>
#include <memory>

#ifndef EUPHORIA_UI_DISPLAY_H_
#define EUPHORIA_UI_DISPLAY_H_

namespace euphoria {
namespace ui {
class Row;

class Display {
 public:
  void Render();

 private:
  std::vector<std::shared_ptr<Row>> rows_;
};
}  // namespace ui
}  // namespace euphoria

#endif  // EUPHORIA_UI_DISPLAY_H_
