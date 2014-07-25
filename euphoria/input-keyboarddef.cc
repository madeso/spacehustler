// Euphoria - Copyright (c) Gustav

#include "euphoria/input-keyboarddef.h"
#include <cassert>
#include <stdexcept>
#include <string>

#include "euphoria/str.h"
#include "json/json.h"

#include "euphoria/input-keyboardactiveunit.h"
#include "euphoria/input-bindmap.h"
#include "euphoria/stringutils.h"
#include "euphoria/input-commondef.h"

namespace input {

KeyboardDef::KeyboardDef(const Json::Value& data, const InputActionMap& map) {
  assert(this);

  for (Json::ArrayIndex i = 0; i < data.size(); ++i) {
    Json::Value d = data[i];

    auto common = GetCommonDef(d, map);

    if (common.type == "button") {
      const std::string keyname = d.get("key", "").asString();
      const auto key = Key::FromString(keyname);

      if (key == Key::Invalid) {
        auto error = (Str() << keyname << " is a invalid key for the "
                            << common.bindname << " action").ToString();
        throw error;
      }
      keys_.push_back(BindDef<Key::Type>(common.bindname, key));
    } else {
      std::string error =
          Str() << "Unknown input type for keyboard: " << common.type;
      throw error;
    }
  }
}

std::shared_ptr<ActiveUnit> KeyboardDef::Create(InputDirector* director,
                                                BindMap* map) {
  assert(this);
  assert(director);
  assert(map);

  std::vector<std::shared_ptr<TRangeBind<Key::Type>>> keybinds;
  for (const auto& key : keys_) {
    std::shared_ptr<TRangeBind<Key::Type>> b(
        new TRangeBind<Key::Type>(key.type(), map->range(key.id())));
    keybinds.push_back(b);
  }

  std::shared_ptr<ActiveUnit> unit(new KeyboardActiveUnit(keybinds, director));
  return unit;
}

}  // namespace input
