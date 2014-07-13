// Euphoria - Copyright (c) Gustav

#include "euphoria/input-activemasteraxis.h"
#include <cassert>

namespace input {

ActiveMasterAxis::ActiveMasterAxis(InputAction* action) : InputActive(action) {}

void ActiveMasterAxis::update(float dt) { assert(this); }

}  // namespace input
