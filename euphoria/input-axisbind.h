// Euphoria - Copyright (c) Gustav

/** @file
Classes for input handling.
 */

#ifndef EUPHORIA_INPUT_AXISBIND_H_
#define EUPHORIA_INPUT_AXISBIND_H_

#include "euphoria/input-axisdata.h"

#include <cassert>

namespace Json {
class Value;
}

namespace input {

class InputAction;

/** For use in template/binding.
*/
template <typename Type>
class AxisBind : public AxisData {
 public:
  /** Constructor.
  @param type the axis type
  @param action the action
  @param data the data to setup from
   */
  AxisBind(Type type, std::shared_ptr<InputAction> action,
           const Json::Value& data)
      : type_(type), AxisData(action, data) {
    assert(this);
  }

  /** Get the axis type.
  @returns the axis type
   */
  Type type() const {
    assert(this);
    return type_;
  }

 private:
  Type type_;
};

}  // namespace input

#endif  // EUPHORIA_INPUT_AXISBIND_H_
