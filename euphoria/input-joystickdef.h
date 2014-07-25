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

/** Joystick definition.
 */
class JoystickDef : public UnitDef {
 public:
  /** Constructor.
  @param data the data structure to load from
   */
  explicit JoystickDef(const Json::Value& data);

  /** Create a active joystick interface.
  @param director the input director
  @returns the active joystick
   */
  std::shared_ptr<ActiveUnit> Create(InputDirector* director, BindMap* map);

 private:
  /// @todo add joystick binds
};

}  // namespace input

#endif  // EUPHORIA_INPUT_JOYSTICKDEF_H_
