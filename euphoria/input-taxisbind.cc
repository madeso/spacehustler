// Euphoria - Copyright (c) Gustav

#include "euphoria/input-taxisbind.h"
#include "euphoria/input-action.h"
namespace input {

namespace {
void test() {
  InputAction action("test", Range::Infinite);
  TAxisBind<int> test(4, &action);
}
}

}  // namespace input
