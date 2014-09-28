// Euphoria - Copyright (c) Gustav

#include "euphoria/ui-container-table.h"

#include <cassert>

#include "euphoria/ui-value.h"
#include "euphoria/scalar.h"
#include "euphoria/ui-widget.h"

namespace euphoria {
namespace ui {

//////////////////////////////////////////////////////////////////////////

Cell::Cell(std::shared_ptr<Container> container, const Vec2i& position,
           const Vec2i& size)
    : container_(container), position_(position), size_(size) {
  assert(this);
}

Vec2i Cell::tile_position() const {
  assert(this);
  return position_;
}

Vec2i Cell::tile_size() const {
  assert(this);
  return size_;
}

void Cell::SetPositionAndSize(Vec2 position, Vec2 size) const {
  assert(this);
  assert(container_);
  container_->set_position(position);
  container_->set_size(size);
  container_->Layout();
}

void Cell::Draw(const Camera& camera) const {
  assert(this);
  assert(container_);
  container_->Draw(camera);
}

Size::Size() { assert(this); }

//////////////////////////////////////////////////////////////////////////

TableContainer::TableContainer() { assert(this); }

TableContainer::~TableContainer() { assert(this); }

const std::vector<Size>& TableContainer::rows() const {
  assert(this);
  return rows_;
}

void TableContainer::set_rows(const std::vector<Size>& rows) {
  assert(this);
  rows_ = rows;
}

const std::vector<Size>& TableContainer::columns() const {
  assert(this);
  return columns_;
}

void TableContainer::set_columns(const std::vector<Size>& columns) {
  assert(this);
  columns_ = columns;
}

void TableContainer::AddColoumn(Size column) {
  assert(this);
  columns_.push_back(column);
}

void TableContainer::AddRow(Size row) {
  assert(this);
  rows_.push_back(row);
}

void TableContainer::AddCell(Cell cell) {
  assert(this);
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

float CalculatePositionValue(Cell cell, const std::vector<float>& sizes,
                             int index) {
  float ret = 0;
  for (int i = 0; i < cell.tile_position()[index]; ++i) {
    ret += sizes[i];
  }
  return ret;
}

float CalculateSizeValue(Cell cell, const std::vector<float>& sizes,
                         int index) {
  float ret = 0;
  int p = cell.tile_position()[index];
  for (int i = 0; i < cell.tile_size()[index]; ++i) {
    ret += sizes[p];
    ++p;
  }
  return ret;
}
}  // namespace

void TableContainer::Layout() {
  assert(this);

  const float width = size()[0];
  const float height = size()[1];

  assert(width > 0.0f);
  assert(height > 0.0f);

  const std::vector<float> widths = CalculateSizes(columns_, width);
  const std::vector<float> heights = CalculateSizes(rows_, height);

  for (const auto& c : cells_) {
    // subtract width/height divided by half b/c 0,0 is in the middle
    const Vec2 p = Vec2(CalculatePositionValue(c, widths, 0) - width / 2,
                        height / 2 - CalculatePositionValue(c, heights, 1));
    const Vec2 s = Vec2(CalculateSizeValue(c, widths, 0),
                        CalculateSizeValue(c, heights, 1));
    c.SetPositionAndSize(position() + p, s);
  }
}

void TableContainer::Draw(const Camera& camera) {
  assert(this);
  for (const auto& c : cells_) {
    c.Draw(camera);
  }
}

}  // namespace ui
}  // namespace euphoria
