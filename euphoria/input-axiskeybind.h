// Euphoria - Copyright (c) Gustav

/** @file
Classes for input handling.
 */

#ifndef EUPHORIA_INPUT_AXISKEYBIND_H_
#define EUPHORIA_INPUT_AXISKEYBIND_H_

#include "euphoria/input-axisbase.h"

#include <cassert>

namespace input {

/** For use in template/binding.
*/
template <typename Type>
class AxisKeyBind : public AxisBase {
 public:
  AxisKeyBind(Type min, Type max, std::shared_ptr<InputAction> action,
              const Json::Value& data)
      : AxisBase(action, data), min_(min), max_(max) {
    assert(this);
  }

  Type min() const {
    assert(this);
    return min_;
  }

  Type max() const {
    assert(this);
    return max_;
  }

 private:
  Type min_;
  Type max_;
};

}  // namespace input

#endif  // EUPHORIA_INPUT_AXISKEYBIND_H_
