// Euphoria - Copyright (c) Gustav

#include "euphoria/input-joystickdef.h"
#include <cassert>
#include <string>

#include "json/json.h"

#include "euphoria/input-dummyactiveunit.h"

#include "euphoria/str.h"
#include "euphoria/input-actionmap.h"
#include "euphoria/input-bindmap.h"
#include "euphoria/input-commondef.h"

namespace input {

JoystickDef::JoystickDef(const Json::Value& data, const InputActionMap& map) {
  assert(this);
  for (Json::ArrayIndex i = 0; i < data.size(); ++i) {
    Json::Value d = data[i];
    auto common = GetCommonDef(d, map);

    if (common.type == "axis") {
      const int axis = d.get("axis", -1).asInt();
      if (axis < 0) {
        const std::string error = Str() << "Invalid joystick axis for "
                                        << common.bindname << " action";
        throw error;
      }
      axis_.push_back(BindDef<int>(common.bindname, axis));
    } else if (common.type == "button") {
      const int key = d.get("button", -1).asInt();

      if (key < 0) {
        const std::string error = Str() << "Invalid joystick button for the "
                                        << common.bindname << " action";
        throw error;
      }
      buttons_.push_back(BindDef<int>(common.bindname, key));
    } else if (common.type == "hat") {
      const int hat = d.get("hat", -1).asInt();
      if (hat < 0) {
        const std::string error = Str() << "Invalid joystick hat for the "
                                        << common.bindname << " action";
        throw error;
      }

      const std::string axisname = d.get("axis", "").asString();
      const auto axis = Axis::FromString(axisname);
      if (axis == Axis::Invalid) {
        const std::string error = Str() << "Invalid axis " << axisname;
        throw error;
      }

      hats_.push_back(BindDef<HatAxis>(common.bindname, HatAxis(hat, axis)));
    } else {
      std::string error =
          Str() << "Unknown input type for mouse: " << common.type;
      throw error;
    }
  }
}

std::shared_ptr<ActiveUnit> JoystickDef::Create(InputDirector* director,
                                                BindMap* map) {
  std::shared_ptr<ActiveUnit> unit(new DummyActiveUnit());
  return unit;
}

}  // namespace input
