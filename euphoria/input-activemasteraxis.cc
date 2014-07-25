// Euphoria - Copyright (c) Gustav

#include "euphoria/input-activemasteraxis.h"
#include <cassert>

#include "euphoria/input-activeaxis.h"
#include "euphoria/input-activerangetoaxis.h"

namespace input {

ActiveMasterAxis::ActiveMasterAxis(InputAction* action, ActiveAxis* axis,
                                   ActiveRangeToAxis* range)
    : InputActive(action), axis_(axis), range_(range) {
  assert(this);
  assert(axis_);
  assert(range_);
}

void ActiveMasterAxis::update(float dt) {
  assert(this);

  const float axisvalue = axis_->state();
  const float rangevalue = range_->state();

  /// @todo: change to a bind check instead of a value check
  return set_state(axisvalue == 0.0f ? rangevalue : axisvalue);
}

}  // namespace input
