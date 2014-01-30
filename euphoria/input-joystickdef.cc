// Euphoria - Copyright (c) Gustav

#include "euphoria/input-joystickdef.h"
#include <cassert>

#include "json/json.h"

#include "euphoria/input-dummyactiveunit.h"

namespace input {

  JoystickDef::JoystickDef(const Json::Value& data) {}

  std::shared_ptr<ActiveUnit> JoystickDef::Create(InputDirector* director) {
    std::shared_ptr<ActiveUnit> unit(new DummyActiveUnit());
    return unit;
  }

}  // namespace input
