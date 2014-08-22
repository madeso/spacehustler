// Euphoria - Copyright (c) Gustav

#include "euphoria/ui-row.h"

#include <cassert>

namespace euphoria {
namespace ui {

Row::Row() { assert(this); }

Row::~Row() { assert(this); }

void Row::Add(std::shared_ptr<Cell> cell) {
  assert(this);
  assert(cell);
  columns_.push_back(cell);
}

}  // namespace ui
}  // namespace euphoria
