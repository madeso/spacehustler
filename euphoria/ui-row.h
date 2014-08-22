// Euphoria - Copyright (c) Gustav

/** @file
Camera related code.
 */

#ifndef EUPHORIA_UI_ROW_H_
#define EUPHORIA_UI_ROW_H_

#include <vector>
#include <memory>

namespace euphoria {
namespace ui {
class Cell;

class Row {
 public:
  Row();
  ~Row();

  void Add(std::shared_ptr<Cell> cell);

 private:
  std::vector<std::shared_ptr<Cell>> columns_;
};
}  // namespace ui
}  // namespace euphoria

#endif  // EUPHORIA_UI_ROW_H_
