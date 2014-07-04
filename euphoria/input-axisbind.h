// Euphoria - Copyright (c) Gustav

/** @file
Classes for input handling.
 */

#ifndef EUPHORIA_INPUT_AXISBIND_H_
#define EUPHORIA_INPUT_AXISBIND_H_

#include <memory>
#include <cassert>

#include "euphoria/input-bind.h"

namespace input {
class AxisBind : public Bind {
 public:
  explicit AxisBind(Action* action);
  void update(float dt);
};

}  // namespace input

#endif  // EUPHORIA_INPUT_AXISBIND_H_
