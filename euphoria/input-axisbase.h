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

  class AxisBase {
  public:
    AxisBase(std::shared_ptr<InputAction> action, const Json::Value& data);

    std::shared_ptr<InputAction> action() const;

    float scale() const;

    void set_scale(float scale);

    // smoothing?

    float Process(float v) const;

  private:
    std::shared_ptr<InputAction> action_;
    float scale_;
  };

}  // namespace input

#endif  // EUPHORIA_INPUT_AXISBASE_H_
