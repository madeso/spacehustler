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

class Table;

namespace input {

class InputDirector;

/** A active mouse binding
 */
class MouseActiveUnit : public ActiveUnit {
 public:
  /** Constructor.
  @param axis the axis binds to use
  @param director the input director
   */
  MouseActiveUnit(const std::vector<std::shared_ptr<TAxisBind<Axis>>>& axis,
                  const std::vector<
                      std::shared_ptr<TRangeBind<MouseButton::Type>>>& buttons,
                  InputDirector* director);

  /** React to a change in the axis.
  @param key the axis
  @param state the state of the axis
   */
  void OnAxis(const Axis& key, float state);

  void OnButton(MouseButton::Type key, float state);

  /** Destructor.
   */
  ~MouseActiveUnit();

  /** Rumble the mouse.
  Doesn't do anything.
   */
  void Rumble();

 private:
  InputDirector* director_;
  const std::map<Axis, std::shared_ptr<Bind>> actions_;
  const std::map<MouseButton::Type, std::shared_ptr<Bind>> buttons_;
};

}  // namespace input

#endif  // EUPHORIA_INPUT_MOUSEACTIVEUNIT_H_
