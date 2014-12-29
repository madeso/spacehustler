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

namespace euphoria {

namespace input {

KeyboardDef::KeyboardDef(const Json::Value& data, const InputActionMap& map) {
  assert(this);

  for (Json::ArrayIndex i = 0; i < data.size(); ++i) {
    Json::Value d = data[i];

    auto common = GetCommonDef(d, map);

    if (common.type == "button") {
      const std::string keyname = d.get("key", "").asString();
      const auto key = ToKey(keyname);

      if (key == Key::INVALID) {
        auto error = (Str() << keyname << " is a invalid key for the "
                            << common.bindname << " action").ToString();
        throw error;
      }
      keys_.push_back(BindDef<Key>(common.bindname, key, d));
    } else {
      std::string error = Str()
                          << "Unknown input type for keyboard: " << common.type;
      throw error;
    }
  }
}

std::shared_ptr<ActiveUnit> KeyboardDef::Create(InputDirector* director,
                                                BindMap* map) {
  assert(this);
  assert(director);
  assert(map);

  std::vector<std::shared_ptr<TRangeBind<Key>>> keybinds =
      CreateBinds<TRangeBind<Key>, Key>(keys_, map);

  std::shared_ptr<ActiveUnit> unit(new KeyboardActiveUnit(keybinds, director));
  return unit;
}

}  // namespace input
}  // namespace euphoria
