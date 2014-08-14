// Euphoria - Copyright (c) Gustav

#include "euphoria/input-activemasterrange.h"
#include <cassert>

#include "euphoria/input-activerange.h"
#include "euphoria/input-activeaxistorange.h"

namespace euphoria {

namespace input {

ActiveMasterRange::ActiveMasterRange(InputAction* action, ActiveRange* range,
                                     ActiveAxisToRange* axis)
    : InputActive(action), range_(range), axis_(axis) {
  assert(this);
  assert(range_);
  assert(axis_);
}

void ActiveMasterRange::Update(float dt) {
  assert(this);

  const float axisvalue = axis_->state();
  const float rangevalue = range_->state();

  return set_state(rangevalue + axisvalue);
}

}  // namespace input
}  // namespace euphoria
