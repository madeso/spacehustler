// Euphoria - Copyright (c) Gustav

/** @file
Classes for input handling.
 */

#ifndef EUPHORIA_INPUT_ACTIVEMASTERRANGE_H_
#define EUPHORIA_INPUT_ACTIVEMASTERRANGE_H_

#include <memory>
#include <cassert>

#include "euphoria/input-active.h"

namespace input {

class ActiveRange;
class ActiveAxisToRange;

class ActiveMasterRange : public InputActive {
 public:
  explicit ActiveMasterRange(InputAction* action, ActiveRange* range,
                             ActiveAxisToRange* axis);
  void update(float dt);

 private:
  ActiveRange* range_;
  ActiveAxisToRange* axis_;
};

}  // namespace input

#endif  // EUPHORIA_INPUT_ACTIVEMASTERRANGE_H_
