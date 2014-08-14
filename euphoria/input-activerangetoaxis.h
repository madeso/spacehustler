// Euphoria - Copyright (c) Gustav

/** @file
Classes for input handling.
 */

#ifndef EUPHORIA_INPUT_ACTIVERANGETOAXIS_H_
#define EUPHORIA_INPUT_ACTIVERANGETOAXIS_H_

#include "euphoria/input-active.h"

namespace euphoria {

namespace input {

class InputAction;
class Bind;

/** A input active range.
 */
class ActiveRangeToAxis : public InputActive {
 public:
  /** Constructor.
  @param action the action this active belongs to
   */
  ActiveRangeToAxis(InputAction* action, Bind* positive, Bind* negative);

  void Update(float dt);

 private:
  Bind* positive_;
  Bind* negative_;
};

}  // namespace input

}  // namespace euphoria

#endif  // EUPHORIA_INPUT_ACTIVERANGETOAXIS_H_
