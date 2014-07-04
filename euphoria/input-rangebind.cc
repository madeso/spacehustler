// Euphoria - Copyright (c) Gustav

#include "euphoria/input-rangebind.h"
#include <cassert>

namespace input {

RangeBind::RangeBind(Action* action) : Bind(action) { assert(this); }

void RangeBind::update(float dt) {
  assert(this);

  /// @todo use set_value() based on current active, smoothed values?
}

}  // namespace input
