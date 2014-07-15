// Euphoria - Copyright (c) Gustav

#include "euphoria/input-binddef.h"

#include "euphoria/lua.h"

namespace input {

namespace {
  void test() {
    BindDef<int> def("dog", 1);
  }
}

}  // namespace input
