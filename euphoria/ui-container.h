// Euphoria - Copyright (c) Gustav

/** @file
Camera related code.
 */

#ifndef EUPHORIA_UI_CONTAINER_H_
#define EUPHORIA_UI_CONTAINER_H_

#include <vector>
#include <memory>

#include "euphoria/math.h"

namespace euphoria {

class Camera;

namespace ui {

/** Something to be placed in a 2d world.
The difference between a container and a widget is that after it has received
it's position and size from a layout call, it may never change its own size.
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
class Container {
 public:
  Container();
  virtual ~Container();

  Vec2 position() const;
  void set_position(const Vec2& position);

  Vec2 size() const;
  void set_size(const Vec2& size);

  virtual void Layout() = 0;
  virtual void Draw(const Camera& camera) = 0;

 private:
  Vec2 position_;
  Vec2 size_;
};

}  // namespace ui
}  // namespace euphoria

#endif  // EUPHORIA_UI_CONTAINER_H_
