// Euphoria - Copyright (c) Gustav

/** @file
Camera related code.
 */

#ifndef EUPHORIA_UI_TABLE_H_
#define EUPHORIA_UI_TABLE_H_

#include <vector>
#include <memory>
#include <string>

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

  const std::vector<Size>& rows() const;
  void set_rows(const std::vector<Size>& rows);

  const std::vector<Size>& columns() const;
  void set_columns(const std::vector<Size>& columns);

  void AddColoumn(Size column);
  void AddRow(Size row);
  void AddCell(std::shared_ptr<Cell> cell);

  void LayoutCells(float width, float height);

 private:
  std::vector<Size> rows_;
  std::vector<Size> columns_;
  std::vector<std::shared_ptr<Cell>> cells_;
};

void LoadTable(Table* table, const std::string& file);

}  // namespace ui
}  // namespace euphoria

#endif  // EUPHORIA_UI_TABLE_H_
