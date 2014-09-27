// Euphoria - Copyright (c) Gustav

/** @file
Camera related code.
 */

#ifndef EUPHORIA_UI_WIDGET_H_
#define EUPHORIA_UI_WIDGET_H_

#include "euphoria/math.h"
#include "euphoria/ui-padding.h"

namespace euphoria {

class Camera;

namespace ui {

/** A Widget to be placed in 2d.
  */

/**
Positive Y is up and, positive X is right. Relative (0,0) is upper left.
<pre>
 (0,0)   w
   +-----------+
   |           |
 h |           |
   |           |
   +-----------+

   +
   ^
   |
   y / x ---> +
</pre>
*/
class Widget {
 public:
  Widget();
  virtual ~Widget();

  /** Draw the widget.
  The widget is responsible for using its padding, size, sizerule, alignment
  @see GetPositionAndSize
   */

  virtual void Draw(const Camera& camera) = 0;

  Vec2 position() const;
  void set_position(const Vec2& position);

  Vec2 size() const;
  void set_size(const Vec2& size);

  Padding padding() const;
  void set_padding(const Padding& padding);

  SizeRule sizerule() const;
  void set_sizerule(SizeRule sizerule);

  Alignment alignment() const;
  void set_alignment(Alignment alignment);
 private:
  Vec2 position_;
  Vec2 size_;
  Padding padding_;

  SizeRule sizerule_;
  Alignment alignment_;
};

std::pair<Vec2,Vec2> GetPositionAndSize(const Vec2 position, const Vec2 size, const Padding& padding, SizeRule sizerule, Alignment alignment, const Vec2& minimumSize);
}  // namespace ui
}  // namespace euphoria

#endif  // EUPHORIA_UI_WIDGET_H_
