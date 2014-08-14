// Euphoria - Copyright (c) Gustav

#include "euphoria/input-taxisbind.h"
#include "euphoria/input-action.h"
namespace euphoria {
namespace input {

namespace {
void test() {
  InputAction action("test", "test", Range::INFINITE, false);
  std::shared_ptr<Bind> bind(new Bind(&action, BindType::AXIS));
  TAxisBind<int> test(4, bind, false, 1.0f);
}
}

}  // namespace input
}  // namespace euphoria
