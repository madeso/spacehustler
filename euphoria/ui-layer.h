// Euphoria - Copyright (c) Gustav

/** @file
Camera related code.
 */

#ifndef EUPHORIA_UI_LAYER_H_
#define EUPHORIA_UI_LAYER_H_

#include <vector>
#include <memory>

namespace euphoria {
namespace ui {
class Widget;
class Layout;

class Layer {
 public:
  Layer();
  ~Layer();

 private:
  std::unique_ptr<Layout> layout_;
  std::vector<std::shared_ptr<Widget>> widgets_;
};
}  // namespace ui
}  // namespace euphoria

#endif  // EUPHORIA_UI_LAYER_H_
