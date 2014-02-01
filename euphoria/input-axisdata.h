// Euphoria - Copyright (c) Gustav

/** @file
Classes for input handling.
 */

#ifndef EUPHORIA_INPUT_AXISDATA_H_
#define EUPHORIA_INPUT_AXISDATA_H_

#include <memory>

#include "euphoria/input-axisbase.h"

class Table;

namespace Json {
class Value;
}

namespace input {
class InputAction;

/** For use during activity.
*/
class AxisData : public AxisBase {
 public:
   /** Constructor.
   @param action the action
   @param data teh data to read from
    */
  AxisData(std::shared_ptr<InputAction> action, const Json::Value& data);

  /** Process the input according to the settings.
  @param v the input
  @returns the processed output
   */
  float Process(float v) const;

  // nonlinear
  // correction

 private:
  bool invert_;
  float deadzone_;
};

}  // namespace input

#endif  // EUPHORIA_INPUT_AXISDATA_H_
