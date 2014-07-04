// Euphoria - Copyright (c) Gustav

/** @file
Classes for input handling.
 */

#ifndef EUPHORIA_INPUT_MOUSEACTIVEUNIT_H_
#define EUPHORIA_INPUT_MOUSEACTIVEUNIT_H_

#include <vector>
#include <map>

#include "euphoria/input-axisdata.h"
#include "euphoria/input-axis.h"
#include "euphoria/input-axisbind.h"
#include "euphoria/input-activeunit.h"

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
  MouseActiveUnit(const std::vector<AxisBind<Axis::Type>>& axis,
                  InputDirector* director);

  /** React to a change in the axis.
  @param key the axis
  @param state the state of the axis
   */
  void OnAxis(const Axis::Type& key, float state);

  /** Destructor.
   */
  ~MouseActiveUnit();

  /** Rumble the mouse.
  Doesn't do anything.
   */
  void Rumble();

 private:
  InputDirector* director_;
  std::map<Axis::Type, AxisData> actions_;
};

}  // namespace input

#endif  // EUPHORIA_INPUT_MOUSEACTIVEUNIT_H_
