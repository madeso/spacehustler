// Euphoria - Copyright (c) Gustav

/** @file
Camera related code.
 */

#ifndef EUPHORIA_UI_TABLE_H_
#define EUPHORIA_UI_TABLE_H_

#include <vector>
#include <memory>

namespace euphoria {
namespace ui {
class Row;

class Table {
 public:
  Table();
  ~Table();

 private:
  std::vector<std::shared_ptr<Row>> rows_;
};
}  // namespace ui
}  // namespace euphoria

#endif  // EUPHORIA_UI_TABLE_H_
