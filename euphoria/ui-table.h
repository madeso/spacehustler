// Euphoria - Copyright (c) Gustav

/** @file
Camera related code.
 */

#ifndef EUPHORIA_UI_TABLE_H_
#define EUPHORIA_UI_TABLE_H_

#include <vector>
#include <memory>

#include "euphoria/ui-value.h"

namespace euphoria {
namespace ui {

class Cell;

struct Size {
 public:
  Size();
  Value size;
};

class Table {
 public:
  Table();
  ~Table();

  void AddColoumn(std::shared_ptr<Size> column);

  void AddRow(std::shared_ptr<Size> row);

  void AddCell(std::shared_ptr<Cell> cell);

  void LayoutCells();

 private:
  std::vector<std::shared_ptr<Size>> rows_;
  std::vector<std::shared_ptr<Size>> columns_;
  std::vector<std::shared_ptr<Cell>> cells_;
};
}  // namespace ui
}  // namespace euphoria

#endif  // EUPHORIA_UI_TABLE_H_
