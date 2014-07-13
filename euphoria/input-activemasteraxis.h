// Euphoria - Copyright (c) Gustav

/** @file
Classes for input handling.
 */

#ifndef EUPHORIA_INPUT_ACTIVEMASTERAXIS_H_
#define EUPHORIA_INPUT_ACTIVEMASTERAXIS_H_

#include <memory>
#include <cassert>

#include "euphoria/input-active.h"

namespace input {
class ActiveMasterAxis : public InputActive {
 public:
  explicit ActiveMasterAxis(InputAction* action);
  void update(float dt);
};

}  // namespace input

#endif  // EUPHORIA_INPUT_ACTIVEMASTERAXIS_H_
