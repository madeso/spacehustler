// Euphoria - Copyright (c) Gustav

/** @file
Classes for input handling.
 */

#ifndef EUPHORIA_INPUT_JOYSTICKACTIVEUNIT_H_
#define EUPHORIA_INPUT_JOYSTICKACTIVEUNIT_H_

#include <vector>
#include <map>

#include "euphoria/input-axis.h"
#include "euphoria/input-hataxis.h"
#include "euphoria/input-activeunit.h"
#include "euphoria/input-taxisbind.h"
#include "euphoria/input-trangebind.h"

namespace euphoria {

class Table;

namespace input {

struct BindData;
class InputDirector;

/** A active mouse binding
 */
class JoystickActiveUnit : public ActiveUnit {
 public:
  /** Constructor.
  @param axis the axis binds to use
  @param director the input director
   */
  JoystickActiveUnit(
      int joystick, InputDirector* director,
      const std::vector<std::shared_ptr<TAxisBind<int>>>& axis,
      const std::vector<std::shared_ptr<TRangeBind<int>>>& buttons,
      const std::vector<std::shared_ptr<TAxisBind<HatAxis>>>& hats);

  /** React to a change in the axis.
  @param axis the axis
  @param state the state of the axis
   */
  void OnAxis(int axis, float state);

  void OnButton(int button, float state);

  void OnHat(const HatAxis& hatAxis, float state);

  /** Destructor.
   */
  ~JoystickActiveUnit();

  /** Rumble the joystick.
  Doesn't do anything.
   */
  void Rumble() override;

 private:
  int joystick_;
  InputDirector* director_;
  std::map<int, BindData> axis_;
  std::map<int, BindData> buttons_;
  std::map<HatAxis, BindData> hats_;
};

}  // namespace input

}  // namespace euphoria

#endif  // EUPHORIA_INPUT_JOYSTICKACTIVEUNIT_H_
