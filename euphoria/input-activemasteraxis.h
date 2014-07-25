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

class ActiveAxis;
class ActiveRangeToAxis;

class ActiveMasterAxis : public InputActive {
 public:
  explicit ActiveMasterAxis(InputAction* action, ActiveAxis* axis,
                            ActiveRangeToAxis* range);
  void update(float dt);

 private:
  ActiveAxis* axis_;
  ActiveRangeToAxis* range_;
};

}  // namespace input

#endif  // EUPHORIA_INPUT_ACTIVEMASTERAXIS_H_
