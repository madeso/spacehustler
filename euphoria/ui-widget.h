// Euphoria - Copyright (c) Gustav

/** @file
Camera related code.
 */

#ifndef EUPHORIA_UI_WIDGET_H_
#define EUPHORIA_UI_WIDGET_H_

#include <utility>

#include "euphoria/ui-container.h"
#include "euphoria/ui-enums.h"

namespace euphoria {

class Camera;

namespace ui {

/** A Widget to be placed in 2d.
The difference between a widget and a container is that the widget may reduce
it's size after a layout.
  */
class Widget : public Container {
 public:
  Widget();
  virtual ~Widget();

  SizeRule sizerule() const;
  void set_sizerule(SizeRule sizerule);

  Alignment alignment() const;
  void set_alignment(Alignment alignment);

 private:
  SizeRule sizerule_;
  Alignment alignment_;
};

std::pair<Vec2, Vec2> GetPositionAndSize(const Vec2 position, const Vec2 size,
                                         SizeRule sizerule, Alignment alignment,
                                         const Vec2& minimumSize);
}  // namespace ui
}  // namespace euphoria

#endif  // EUPHORIA_UI_WIDGET_H_
