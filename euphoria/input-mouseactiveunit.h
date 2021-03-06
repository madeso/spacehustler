// Euphoria - Copyright (c) Gustav

/** @file
Classes for input handling.
 */

#ifndef EUPHORIA_INPUT_MOUSEACTIVEUNIT_H_
#define EUPHORIA_INPUT_MOUSEACTIVEUNIT_H_

#include <vector>
#include <map>

#include "euphoria/input-axis.h"
#include "euphoria/input-key.h"
#include "euphoria/input-activeunit.h"
#include "euphoria/input-taxisbind.h"
#include "euphoria/input-trangebind.h"

namespace euphoria {

class Table;

namespace input {

class InputDirector;
struct BindData;

/** A active mouse binding
 */
class MouseActiveUnit : public ActiveUnit {
 public:
  /** Constructor.
  @param axis the axis binds to use
  @param director the input director
   */
  MouseActiveUnit(
      const std::vector<std::shared_ptr<TAxisBind<Axis>>>& axis,
      const std::vector<std::shared_ptr<TRangeBind<MouseButton>>>& buttons,
      InputDirector* director);

  /** React to a change in the axis.
  @param key the axis
  @param state the state of the axis
   */
  void OnAxis(const Axis& key, float state);

  void OnButton(MouseButton key, float state);

  /** Destructor.
   */
  ~MouseActiveUnit();

  /** Rumble the mouse.
  Doesn't do anything.
   */
  void Rumble() override;

 private:
  InputDirector* director_;
  const std::map<Axis, BindData> actions_;
  const std::map<MouseButton, BindData> buttons_;
};

}  // namespace input

}  // namespace euphoria

#endif  // EUPHORIA_INPUT_MOUSEACTIVEUNIT_H_
