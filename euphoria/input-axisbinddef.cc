// Euphoria - Copyright (c) Gustav

#include "euphoria/input-axisbinddef.h"
#include <cassert>

#include "euphoria/input-axisbind.h"

namespace input {

AxisBindDef::AxisBindDef(InputAction* action) : action_(action) {
  assert(this);
  assert(action);
}

std::shared_ptr<AxisBind> AxisBindDef::create() {
  assert(this);
  std::shared_ptr<AxisBind> ret(new AxisBind(action_));
  return ret;
}

}  // namespace input
