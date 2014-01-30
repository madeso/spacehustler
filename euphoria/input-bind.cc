// Euphoria - Copyright (c) Gustav

#include "euphoria/input-bind.h"

namespace input {

  namespace {
    void test() {
      std::shared_ptr<InputAction> action;
      Bind<int> b(2, action);
    }
  }  // namespace

}  // namespace input
