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
  HatAxis(int hat, Axis axis);

  Axis axis() const;
  int hat() const;

  bool operator<(const HatAxis& rhs) const;

 private:
  int hat_;
  Axis axis_;
};

}  // namespace input

#endif  // EUPHORIA_INPUT_HATAXIS_H_
