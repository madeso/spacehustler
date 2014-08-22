// Euphoria - Copyright (c) Gustav

#include "euphoria/ui-cell.h"

#include <cassert>

#include "euphoria/ui-layout.h"
#include "euphoria/ui-widget.h"

namespace euphoria {
namespace ui {

Cell::Cell() : is_layout_paused_(false) { assert(this); }

Cell::~Cell() { assert(this); }

Vec2 Cell::position() const {
  assert(this);
  return position_;
}

void Cell::set_position(Vec2 position) {
  assert(this);
  position_ = position;
  PerformLayout();
}

Vec2 Cell::size() const {
  assert(this);
  return size_;
}

void Cell::set_size(Vec2 size) {
  assert(this);
  size_ = size;
  PerformLayout();
}

void Cell::set_layout(std::shared_ptr<Layout> layout) {
  assert(this);
  layout_ = layout;
  PerformLayout();
}

void Cell::Add(std::shared_ptr<Widget> widget) {
  assert(this);
  widgets_.push_back(widget);
  PerformLayout();
}

void Cell::Begin() {
  assert(this);
  assert(is_layout_paused_ == false);
  is_layout_paused_ = true;
}

void Cell::End() {
  assert(this);
  assert(is_layout_paused_ == true);
  is_layout_paused_ = false;
  PerformLayout();
}

void Cell::Draw() {
  assert(is_layout_paused_ == false);
  for (auto w : widgets_) {
    w->Draw();
  }
}

void Cell::PerformLayout() {
  assert(this);
  if (is_layout_paused_) return;
  if (layout_ == nullptr) return;
  layout_->Perform(widgets_, position_, size_);
}

}  // namespace ui
}  // namespace euphoria
