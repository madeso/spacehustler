// Euphoria - Copyright (c) Gustav

/** @file
Classes for input handling.
 */

#ifndef EUPHORIA_INPUT_TRANGEBIND_H_
#define EUPHORIA_INPUT_TRANGEBIND_H_

#include "euphoria/input-rangebind.h"

namespace input {
template <typename T>
class TRangeBind : public RangeBind {
 public:
  explicit TRangeBind(T button, InputAction* action)
      : RangeBind(action), button_(button) {}

  const T button() const { return button_; }

 private:
  T button_;
};

}  // namespace input

#endif  // EUPHORIA_INPUT_TRANGEBIND_H_
