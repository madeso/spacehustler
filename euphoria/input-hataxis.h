// Euphoria - Copyright (c) Gustav

/** @file
Classes for input definitions.
 */

#ifndef EUPHORIA_INPUT_HATAXIS_H_
#define EUPHORIA_INPUT_HATAXIS_H_

#include "euphoria/input-axis.h"

namespace input {

class HatAxis {
 public:
  HatAxis(int hat, Axis::Type axis);

  Axis::Type axis() const;
  int hat() const;

 private:
  int hat_;
  Axis::Type axis_;
};

}  // namespace input

#endif  // EUPHORIA_INPUT_HATAXIS_H_
