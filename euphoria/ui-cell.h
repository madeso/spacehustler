// Euphoria - Copyright (c) Gustav

/** @file
Camera related code.
 */

#ifndef EUPHORIA_UI_CELL_H_
#define EUPHORIA_UI_CELL_H_

#include <vector>
#include <memory>

namespace euphoria {
namespace ui {
class Layer;

class Cell {
 private:
  // background?
  std::vector<std::shared_ptr<Layer>> layers_;
};
}  // namespace ui
}  // namespace euphoria

#endif  // EUPHORIA_UI_CELL_H_
