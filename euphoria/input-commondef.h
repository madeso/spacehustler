// Euphoria - Copyright (c) Gustav

/** @file
Classes for common input def loading.
 */

#ifndef EUPHORIA_INPUT_COMMONDEF_H_
#define EUPHORIA_INPUT_COMMONDEF_H_

#include <string>
#include <vector>
#include <memory>

#include "euphoria/input-binddef.h"

namespace Json {
class Value;
}

namespace input {
class InputActionMap;
class BindMap;

struct CommonDef {
  std::string bindname;
  std::string actionname;
  std::string type;
};

CommonDef GetCommonDef(Json::Value& d, const InputActionMap& map);

template <typename TBind, typename Type>
std::vector<std::shared_ptr<TBind>> CreateBinds(std::vector<BindDef<Type>> defs,
                                                BindMap* map) {
  std::vector<std::shared_ptr<TBind>> keybinds;
  for (const auto& key : defs) {
    std::shared_ptr<TBind> b(new TBind(key.type(), map->bind(key.id())));
    keybinds.push_back(b);
  }
  return keybinds;
}

}  // namespace input

#endif  // EUPHORIA_INPUT_COMMONDEF_H_