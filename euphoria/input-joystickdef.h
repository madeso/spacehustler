// Euphoria - Copyright (c) Gustav

/** @file
Classes for input handling.
 */

#ifndef EUPHORIA_INPUT_JOYSTICKDEF_H_
#define EUPHORIA_INPUT_JOYSTICKDEF_H_

#include <vector>

#include "euphoria/input-unitdef.h"
#include "euphoria/input-binddef.h"
#include "euphoria/input-hataxis.h"

namespace Json {
class Value;
}

namespace euphoria {

namespace input {

class InputDirector;
class ActiveUnit;
class InputActionMap;

/** Joystick definition.
 */
class JoystickDef : public UnitDef {
 public:
  /** Constructor.
  @param data the data structure to load from
   */
  JoystickDef(const Json::Value& data, const InputActionMap& map);

  /** Create a active joystick interface.
  @param director the input director
  @returns the active joystick
   */
  std::shared_ptr<ActiveUnit> Create(InputDirector* director, BindMap* map);

 private:
  std::vector<BindDef<int>> axis_;
  std::vector<BindDef<int>> buttons_;
  std::vector<BindDef<HatAxis>> hats_;
};

}  // namespace input

}  // namespace euphoria

#endif  // EUPHORIA_INPUT_JOYSTICKDEF_H_
