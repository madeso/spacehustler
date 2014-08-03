// Euphoria - Copyright (c) Gustav

#include "euphoria/input-binddef.h"

#include "euphoria/lua.h"

namespace input {

namespace {
void test() {
  Json::Value d;
  BindDef<int> def("dog", 1, d);
}
}

}  // namespace input
