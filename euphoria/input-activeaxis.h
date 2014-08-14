// Euphoria - Copyright (c) Gustav

/** @file
Classes for input handling.
 */

#ifndef EUPHORIA_INPUT_ACTIVEAXIS_H_
#define EUPHORIA_INPUT_ACTIVEAXIS_H_

#include "euphoria/input-active.h"

namespace euphoria {

namespace input {

class InputAction;
class Bind;

/** A input active axis.
 */
class ActiveAxis : public InputActive {
 public:
  /** Constructor.
  @param action the action this active belongs to
   */
  ActiveAxis(InputAction* action, Bind* axis);

  void Update(float dt);

 private:
  Bind* axis_;
};

}  // namespace input

}  // namespace euphoria

#endif  // EUPHORIA_INPUT_ACTIVEAXIS_H_
