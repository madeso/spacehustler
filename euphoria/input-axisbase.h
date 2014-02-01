// Euphoria - Copyright (c) Gustav

/** @file
Classes for input handling.
 */

#ifndef EUPHORIA_INPUT_AXISBASE_H_
#define EUPHORIA_INPUT_AXISBASE_H_

namespace Json {
class Value;
}

#include <memory>

namespace input {

class InputAction;

/** The basis of a axis input.
 */
class AxisBase {
 public:
  /** Constructor.
  @param action the action
  @param data the data to read from
   */
  AxisBase(std::shared_ptr<InputAction> action, const Json::Value& data);

  /** Get the action.
  @returns the action
   */
  std::shared_ptr<InputAction> action() const;

  /** Gets the scale.
  @returns the scale.
   */
  float scale() const;

  /** Sets the scale
  @param scale the new scale
   */
  void set_scale(float scale);

  // smoothing?

  /** Process input according to the settings.
  @param v the value to process
  @returns the processed value
   */
  float Process(float v) const;

 private:
  std::shared_ptr<InputAction> action_;
  float scale_;
};

}  // namespace input

#endif  // EUPHORIA_INPUT_AXISBASE_H_
