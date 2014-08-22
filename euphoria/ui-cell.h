// Euphoria - Copyright (c) Gustav

/** @file
Camera related code.
 */

#ifndef EUPHORIA_UI_CELL_H_
#define EUPHORIA_UI_CELL_H_

#include <vector>
#include <memory>

#include "euphoria/math.h"

namespace euphoria {
namespace ui {
class Widget;
class Layout;

class Cell {
 public:
  Cell();
  ~Cell();

  Vec2 position() const;
  void set_position(Vec2 size);

  Vec2 size() const;
  void set_size(Vec2 size);

  void set_layout(std::shared_ptr<Layout> layout);

  void Add(std::shared_ptr<Widget> widget);
  void Begin();
  void End();

  void Draw();

 private:
  void PerformLayout();

  bool is_layout_paused_;
  Vec2 position_;
  Vec2 size_;
  std::shared_ptr<Layout> layout_;
  std::vector<std::shared_ptr<Widget>> widgets_;
};
}  // namespace ui
}  // namespace euphoria

#endif  // EUPHORIA_UI_CELL_H_
