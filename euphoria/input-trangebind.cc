// Euphoria - Copyright (c) Gustav

#include "euphoria/input-trangebind.h"
#include "euphoria/input-action.h"

namespace input {

namespace {
void test() {
  InputAction action("test", Range::Infinite);
  TRangeBind<int> test(4, &action);
}
}

}  // namespace input
