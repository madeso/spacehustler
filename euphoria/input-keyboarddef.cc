// Euphoria - Copyright (c) Gustav

#include "euphoria/input-keyboarddef.h"
#include <cassert>
#include <stdexcept>
#include <string>

#include "euphoria/str.h"
#include "json/json.h"

#include "euphoria/input-keyboardactiveunit.h"
#include "euphoria/input-actionmap.h"
#include "euphoria/input-bindmap.h"
#include "euphoria/input-action.h"
#include "euphoria/stringutils.h"

namespace input {

KeyboardDef::KeyboardDef(const Json::Value& data, const InputActionMap& map) {
  assert(this);

  for (Json::ArrayIndex i = 0; i < data.size(); ++i) {
    Json::Value d = data[i];

    const std::string bindname = d.get("action", "").asString();

    // verify action
    const std::string actionname =
        RemoveFromEnd(RemoveFromEnd(bindname, "+"), "-");
    const auto action = map.Get(actionname);
    if (actionname != bindname) {
      // bindname is different from actionname, must be a axis
      bool isAxis = action->range() == Range::Infinite ||
                    action->range() == Range::WithinNegative1Positive1;
      if (!isAxis) {
        throw "bind looks like a axis but isn't";
      }
    }

    const std::string type = d.get("type", "").asString();

    if (type == "button") {
      const std::string keyname = d.get("key", "").asString();
      const auto key = Key::FromString(keyname);

      if (key == Key::Invalid) {
        auto error = (Str() << keyname << " is a invalid key for the "
                            << bindname << " action").ToString();
        throw error;
      }
      keys_.push_back(BindDef<Key::Type>(bindname, key));
    } else {
      std::string error = Str() << "Unknown input type for keyboard: " << type;
      throw error;
    }
  }
}

std::shared_ptr<ActiveUnit> KeyboardDef::Create(InputDirector* director,
                                                BindMap* map) {
  assert(this);
  assert(director);
  assert(map);

  std::vector<std::shared_ptr<TRangeBind<Key::Type>>> binds;

  for (const auto& key : keys_) {
    std::shared_ptr<TRangeBind<Key::Type>> b(
        new TRangeBind<Key::Type>(key.type(), map->range(key.id())));
    binds.push_back(b);
  }

  std::shared_ptr<ActiveUnit> unit(new KeyboardActiveUnit(binds, director));
  return unit;
}

}  // namespace input
