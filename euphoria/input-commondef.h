// Euphoria - Copyright (c) Gustav

/** @file
Classes for common input def loading.
 */

#ifndef EUPHORIA_INPUT_COMMONDEF_H_
#define EUPHORIA_INPUT_COMMONDEF_H_

#include <string>

namespace Json {
class Value;
}

namespace input {
class InputActionMap;

struct CommonDef {
  std::string bindname;
  std::string actionname;
  std::string type;
};

CommonDef GetCommonDef(Json::Value& d, const InputActionMap& map);
}  // namespace input

#endif  // EUPHORIA_INPUT_COMMONDEF_H_
