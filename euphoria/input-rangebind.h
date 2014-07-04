// Euphoria - Copyright (c) Gustav

/** @file
Classes for input handling.
 */

#ifndef EUPHORIA_INPUT_RANGEBIND_H_
#define EUPHORIA_INPUT_RANGEBIND_H_

#include "euphoria/input-bind.h"

namespace input {
class RangeBind : public Bind {
 public:
  explicit RangeBind(Action* action);
  void update(float dt);
};

}  // namespace input

#endif  // EUPHORIA_INPUT_RANGEBIND_H_
