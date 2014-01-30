// Euphoria - Copyright (c) Gustav

#include "euphoria/input-axiskeybind.h"
#include "json/json.h"

namespace input {

namespace {
void compiler_test() {
  std::shared_ptr<InputAction> action;
  const Json::Value data;
  AxisKeyBind<int> a(1, 2, action, data);
}
}  // namespace

}  // namespace input
