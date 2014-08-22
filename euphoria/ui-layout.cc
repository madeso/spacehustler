// Euphoria - Copyright (c) Gustav

#include <cassert>

#include "euphoria/ui-layout.h"
#include "euphoria/ui-widget.h"

namespace euphoria {
namespace ui {

Layout::Layout() { assert(this); }

Layout::~Layout() { assert(this); }

// @todo Implement padding!

namespace {
class NullLayout : public Layout {
  void Perform(std::vector<std::shared_ptr<Widget>> widgets, Vec2 position,
               Vec2 size) override {
    // nop
  }
};

class FillLayout : public Layout {
  void Perform(std::vector<std::shared_ptr<Widget>> widgets, Vec2 position,
               Vec2 size) override {
    for (auto widget : widgets) {
      widget->set_position(position);
      widget->set_size(size);
    }
  }
};

class BoxLayout : public Layout {
 public:
  BoxLayout() : size_index_(1) {}

  void Perform(std::vector<std::shared_ptr<Widget>> widgets, Vec2 position,
               Vec2 size) override {
    const size_t total_widgets = widgets.size();
    float remainingSize = size[size_index_];
    size_t current_widget_index = 0;
    float x = position[0];
    float y = position[1];
    for (auto widget : widgets) {
      const size_t remaining_widgets = total_widgets - current_widget_index;

      // get the size along the axis, assume all widgets will expand fully
      const float suggested_size_single = remainingSize / remaining_widgets;
      const float width = size_index_ == 0 ? suggested_size_single : size[0];
      const float height = size_index_ != 0 ? suggested_size_single : size[1];
      const Vec2 suggested_size(width, height);

      // set size and position
      widget->set_position(Vec2(x, y));
      widget->set_size(suggested_size);

      // move along the axis with the actual size
      const Vec2 size = widget->size();
      const float size_single = size[size_index_];
      (size_index_ == 0 ? x : y) += size_single;
      remainingSize -= size_single;
      ++current_widget_index;
    }
  }

 private:
  // 0 = along x
  // 1 = along y
  int size_index_;
};
}  // namespace

std::shared_ptr<Layout> CreateLayout(LayoutType layout) {
  switch (layout) {
    case euphoria::ui::LayoutType::NULL:
      return std::shared_ptr<Layout>(new NullLayout());
    case euphoria::ui::LayoutType::FILL:
      return std::shared_ptr<Layout>(new FillLayout());
    case euphoria::ui::LayoutType::BOX:
      return std::shared_ptr<Layout>(new BoxLayout());
    default:
      assert(false && "layout not handled");
      return std::shared_ptr<Layout>(new FillLayout());
  }
}

}  // namespace ui
}  // namespace euphoria
