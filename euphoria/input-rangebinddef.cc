// Euphoria - Copyright (c) Gustav

#include "euphoria/input-rangebinddef.h"
#include <cassert>

#include "euphoria/input-rangebind.h"

namespace input {

RangeBindDef::RangeBindDef(Action* action) : action_(action) {
  assert(this);
  assert(action);
}

std::shared_ptr<RangeBind> RangeBindDef::create() {
  assert(this);
  std::shared_ptr<RangeBind> ret(new RangeBind(action_));
  return ret;
}

}  // namespace input
