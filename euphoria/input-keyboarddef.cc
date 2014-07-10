// Euphoria - Copyright (c) Gustav

#include "euphoria/input-keyboarddef.h"
#include <cassert>
#include <stdexcept>
#include <string>

#include "euphoria/str.h"
#include "json/json.h"

#include "euphoria/input-keyboardactiveunit.h"
#include "euphoria/input-actionmap.h"

namespace input {

KeyboardDef::KeyboardDef(const Json::Value& data, const InputActionMap& map) {
  for (Json::ArrayIndex i = 0; i < data.size(); ++i) {
    Json::Value d = data[i];

    const std::string actionname = d.get("action", "").asString();
    const auto action = map.Get(actionname);

    const std::string type = d.get("type", "").asString();

    if (type == "button") {
      const std::string keyname = d.get("key", "").asString();
      const auto key = Key::FromString(keyname);

      if (key == Key::Invalid) {
        auto error = (Str() << keyname << " is a invalid key for the "
                            << actionname << " action").ToString();
        throw error;
      }
      binds_.push_back(Bind<Key::Type>(key, action));
    } else if (type == "axiskeys") {
      const std::string minname = d.get("min", "").asString();
      const auto minkey = Key::FromString(minname);

      if (minkey == Key::Invalid) {
        auto error = (Str() << minname << " is a invalid min key for the "
                            << actionname << " action").ToString();
        throw error;
      }

      const std::string maxname = d.get("max", "").asString();
      const auto maxkey = Key::FromString(maxname);

      if (maxkey == Key::Invalid) {
        auto error = (Str() << maxname << " is a invalid max key for the "
                            << actionname << " action").ToString();
        throw error;
      }
      axiskeys_.push_back(AxisKeyBind<Key::Type>(minkey, maxkey, action, d));
    } else {
      auto error = (Str() << type << " is a invalid key type for the "
                          << actionname << " action").ToString();
      throw error;
    }
  }
}

std::shared_ptr<ActiveUnit> KeyboardDef::Create(InputDirector* director) {
  std::vector<std::shared_ptr<TRangeBind<Key::Type>>> binds;

  std::shared_ptr<TRangeBind<Key::Type>> b(new TRangeBind<Key::Type>());

  std::shared_ptr<ActiveUnit> unit(new KeyboardActiveUnit(binds, director));
  return unit;
}

}  // namespace input
