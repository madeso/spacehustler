// Euphoria - Copyright (c) Gustav

/** @file
Classes for input handling.
 */

#ifndef EUPHORIA_INPUT_MOUSEACTIVEUNIT_H_
#define EUPHORIA_INPUT_MOUSEACTIVEUNIT_H_

#include <vector>
#include <map>

#include "euphoria/input-axisdata.h"
#include "euphoria/axis.h"
#include "euphoria/input-axisbind.h"

class Table;

namespace input {

  class MouseActiveUnit : public ActiveUnit {
  public:
    MouseActiveUnit(const std::vector<AxisBind<Axis::Type>>& axis,
      InputDirector* director);

    void OnAxis(const Axis::Type& key, float state);

    ~MouseActiveUnit();

    void Rumble();

  private:
    InputDirector* director_;
    std::map<Axis::Type, AxisData> actions_;
  };

}  // namespace input

#endif  // EUPHORIA_INPUTSYSTEM_H_
