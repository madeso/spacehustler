// Euphoria - Copyright (c) Gustav

#include "euphoria/input-axisbind.h"
#include "json/json.h"

namespace input {

  namespace {
    void compiler_test() {
      std::shared_ptr<InputAction> action;
      const Json::Value data;
      AxisBind<int> a(1, action, data);
    }
  }  // namespace

}  // namespace input
