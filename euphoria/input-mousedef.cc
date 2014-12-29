// Euphoria - Copyright (c) Gustav

#include "euphoria/input-mousedef.h"
#include <cassert>
#include <string>

#include "euphoria/str.h"
#include "euphoria/input-mouseactiveunit.h"
#include "euphoria/input-actionmap.h"
#include "euphoria/input-bindmap.h"
#include "euphoria/input-commondef.h"
#include "json/json.h"

namespace euphoria {

namespace input {

MouseDef::MouseDef(const Json::Value& data, const InputActionMap& map) {
  for (Json::ArrayIndex i = 0; i < data.size(); ++i) {
    Json::Value d = data[i];
    auto common = GetCommonDef(d, map);

    if (common.type == "axis") {
      const std::string axisname = d.get("axis", "").asString();
      const auto axis = ToAxis(axisname);
      if (axis == Axis::INVALID) {
        const std::string error = Str() << "Invalid axis " << axisname;
        throw error;
      }
      axis_.push_back(BindDef<Axis>(common.bindname, axis, d));
    } else if (common.type == "button") {
      const std::string keyname = d.get("key", "").asString();
      const auto key = ToMouseButton(keyname);

      if (key == MouseButton::INVALID) {
        auto error = (Str() << keyname << " is a invalid key for the "
                            << common.bindname << " action").ToString();
        throw error;
      }
      keys_.push_back(BindDef<MouseButton>(common.bindname, key, d));
    } else {
      std::string error = Str()
                          << "Unknown input type for mouse: " << common.type;
      throw error;
    }
  }
}

std::shared_ptr<ActiveUnit> MouseDef::Create(InputDirector* director,
                                             BindMap* map) {
  assert(this);
  assert(director);
  assert(map);

  std::vector<std::shared_ptr<TAxisBind<Axis>>> axisbinds =
      CreateBinds<TAxisBind<Axis>, Axis>(axis_, map);

  std::vector<std::shared_ptr<TRangeBind<MouseButton>>> keybinds =
      CreateBinds<TRangeBind<MouseButton>, MouseButton>(keys_, map);

  std::shared_ptr<ActiveUnit> unit(
      new MouseActiveUnit(axisbinds, keybinds, director));
  return unit;
}

}  // namespace input
}  // namespace euphoria
