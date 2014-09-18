// Euphoria - Copyright (c) Gustav

#include "euphoria/ui-table.h"

#include <cassert>

#include "euphoria/ui-value.h"
#include "euphoria/scalar.h"
#include "euphoria/ui-cell.h"

namespace euphoria {
namespace ui {
Size::Size() { assert(this); }

//////////////////////////////////////////////////////////////////////////

Table::Table() { assert(this); }

Table::~Table() { assert(this); }

const std::vector<Size>& Table::rows() const {
  assert(this);
  return rows_;
}

void Table::set_rows(const std::vector<Size>& rows) {
  assert(this);
  rows_ = rows;
}

const std::vector<Size>& Table::columns() const {
  assert(this);
  return columns_;
}

void Table::set_columns(const std::vector<Size>& columns) {
  assert(this);
  columns_ = columns;
}

void Table::AddColoumn(Size column) {
  assert(this);
  columns_.push_back(column);
}

void Table::AddRow(Size row) {
  assert(this);
  rows_.push_back(row);
}

void Table::AddCell(std::shared_ptr<Cell> cell) {
  assert(this);
  assert(cell);
  cells_.push_back(cell);
}

namespace {
std::vector<float> CalculateSizes(std::vector<Size> sizes, float total_size) {
  std::vector<float> ret(sizes.size());
  float varying = 0;
  std::vector<size_t> varyings;
  float remaining_size = total_size;

  for (size_t i = 0; i < sizes.size(); ++i) {
    auto s = sizes[i];
    if (s.size.IsRestPercentage()) {
      varying += s.size.GetRestPercentage();
      varyings.push_back(i);
    } else {
      const float calculated_size = s.size.value(total_size);
      remaining_size -= calculated_size;
      ret[i] = calculated_size;
    }
  }

  for (auto i : varyings) {
    const float fraction = sizes[i].size.GetRestPercentage() / varying;
    const float calculated_size = Max(remaining_size * fraction, 0.0f);
    ret[i] = calculated_size;
  }

  return ret;
}

float CalculatePositionValue(std::shared_ptr<Cell> cell,
                             const std::vector<float>& sizes, int index) {
  float ret = 0;
  for (int i = 0; i < cell->tile_position()[index]; ++i) {
    ret += sizes[i];
  }
  return ret;
}

float CalculateSizeValue(std::shared_ptr<Cell> cell,
                         const std::vector<float>& sizes, int index) {
  float ret = 0;
  int p = cell->tile_position()[index];
  for (int i = 0; i < cell->tile_size()[index]; ++i) {
    ret += sizes[p];
    ++p;
  }
  return ret;
}
}  // namespace

void Table::LayoutCells(float width, float height) {
  assert(this);

  assert(width > 0.0f);
  assert(height > 0.0f);

  // either this layout wrong or it renders wrong
  // render start at center screen and goes - 0 +, but this expects
  // that it starts at top and goes down

  const std::vector<float> widths = CalculateSizes(columns_, width);
  const std::vector<float> heights = CalculateSizes(rows_, height);

  for (auto c : cells_) {
    // subtract width/height divided by half b/c 0,0 is in the middle
    const Vec2 p = Vec2(CalculatePositionValue(c, widths, 0) - width / 2,
                        height / 2 - CalculatePositionValue(c, heights, 1));
    const Vec2 s = Vec2(CalculateSizeValue(c, widths, 0),
                        CalculateSizeValue(c, heights, 1));
    c->Begin();
    c->set_position(p);
    c->set_size(s);
    c->End();
  }
}

void Table::Render(const Camera& camera) {
  assert(this);
  for (auto c : cells_) {
    c->Draw(camera);
  }
}

}  // namespace ui
}  // namespace euphoria
