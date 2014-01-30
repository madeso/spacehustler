// Euphoria - Copyright (c) Gustav

/** @file
Classes for input handling.
 */

#ifndef EUPHORIA_INPUT_JOYSTICKDEF_H_
#define EUPHORIA_INPUT_JOYSTICKDEF_H_

#include "euphoria/input-unitdef.h"

namespace Json {
class Value;
}

namespace input {

class InputDirector;
class ActiveUnit;

class JoystickDef : public UnitDef {
 public:
  explicit JoystickDef(const Json::Value& data);

  std::shared_ptr<ActiveUnit> Create(InputDirector* director);

 private:
  /// @todo add joystick binds
};

}  // namespace input

#endif  // EUPHORIA_INPUT_JOYSTICKDEF_H_
