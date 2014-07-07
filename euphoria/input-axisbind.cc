// Euphoria - Copyright (c) Gustav

#include "euphoria/input-axisbind.h"
#include <cassert>

namespace input {

AxisBind::AxisBind(InputAction* action) : Bind(action) { assert(this); }

void AxisBind::update(float dt) {
  assert(this);

  /// @todo use set_value() based on current active, smoothed values?
}

}  // namespace input
