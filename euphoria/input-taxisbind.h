// Euphoria - Copyright (c) Gustav

/** @file
Classes for input handling.
 */

#ifndef EUPHORIA_INPUT_TAXISBIND_H_
#define EUPHORIA_INPUT_TAXISBIND_H_

#include <memory>
#include <cassert>

#include "euphoria/input-axisbind.h"

namespace input {

template <typename T>
class TAxisBind : public AxisBind {
 public:
  TAxisBind(T axis, InputAction* action) : AxisBind(action), axis_(axis) {}

  const T axis() const {
    assert(this);
    return axis_;
  }

 private:
  T axis_;
};

}  // namespace input

#endif  // EUPHORIA_INPUT_TAXISBIND_H_
