// Euphoria - Copyright (c) Gustav

/** @file
Camera related code.
 */

#ifndef EUPHORIA_UI_WIDGET_H_
#define EUPHORIA_UI_WIDGET_H_

namespace euphoria {
namespace ui {
class Widget {
 public:
  Widget();
  virtual ~Widget();
  virtual void Draw() = 0;

 private:
  // position
  // size
};
}  // namespace ui
}  // namespace euphoria

#endif  // EUPHORIA_UI_WIDGET_H_
