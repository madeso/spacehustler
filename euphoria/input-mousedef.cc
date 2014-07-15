// Euphoria - Copyright (c) Gustav

#include "euphoria/input-mousedef.h"
#include <cassert>
#include <string>

#include "euphoria/str.h"
#include "euphoria/input-mouseactiveunit.h"
#include "euphoria/input-actionmap.h"
#include "euphoria/input-bindmap.h"
#include "json/json.h"

namespace input {

MouseDef::MouseDef(const Json::Value& data, const InputActionMap& map) {
  for (Json::ArrayIndex i = 0; i < data.size(); ++i) {
    Json::Value d = data[i];
    const std::string type = d.get("type", "").asString();
    const std::string actionname = d.get("action", "").asString();
    const auto action = map.Get(actionname);

    if (type == "axis") {
      const std::string axisname = d.get("axis", "").asString();
      const auto axis = Axis::FromString(axisname);
      if (axis == Axis::Invalid) {
        const std::string error = Str() << "Invalid axis " << axisname;
        throw error;
      }
      axis_.push_back(BindDef<Axis::Type>(actionname, axis));
    } else {
      std::string error = Str() << "Unknown input type: " << type;
      throw error;
    }
  }
}

std::shared_ptr<ActiveUnit> MouseDef::Create(InputDirector* director,
                                             BindMap* map) {
  assert(this);
  assert(director);
  assert(map);

  std::vector<std::shared_ptr<TAxisBind<Axis::Type>>> binds;

  for (const auto& key : axis_) {
    std::shared_ptr<TAxisBind<Axis::Type>> b(
        new TAxisBind<Axis::Type>(key.type(), map->axis(key.id())));
    binds.push_back(b);
  }

  std::shared_ptr<ActiveUnit> unit(new MouseActiveUnit(binds, director));
  return unit;
}

}  // namespace input
