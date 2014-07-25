// Euphoria - Copyright (c) Gustav

#include "euphoria/input-taxisbind.h"
#include "euphoria/input-action.h"
namespace input {

namespace {
void test() {
  InputAction action("test", "test", Range::Infinite, false);
  std::shared_ptr<Bind> bind(new Bind(&action, BindType::Axis));
  TAxisBind<int> test(4, bind);
}
}

}  // namespace input
