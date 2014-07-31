// Euphoria - Copyright (c) Gustav

/** @file
Classes for common input def loading.
 */

#ifndef EUPHORIA_INPUT_COMMONDEF_H_
#define EUPHORIA_INPUT_COMMONDEF_H_

#include <string>
#include <vector>
#include <memory>
#include <map>

#include "euphoria/input-binddef.h"
#include "euphoria/input-bind.h"

namespace Json {  // no-doxygen-namechecks
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
    std::shared_ptr<TBind> b(
        new TBind(key.type(), map->GetBindByName(key.id())));
    keybinds.push_back(b);
  }
  return keybinds;
}

template <typename TBind, typename Type>
std::map<Type, std::shared_ptr<Bind>> ConvertToBindMap(
    const std::vector<std::shared_ptr<TBind>>& axis) {
  std::map<Type, std::shared_ptr<Bind>> actions_;
  for (auto a : axis) {
    actions_.insert(std::make_pair(a->GetType(), a->bind()));
  }
  return actions_;
}

}  // namespace input

#endif  // EUPHORIA_INPUT_COMMONDEF_H_
