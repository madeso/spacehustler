// Euphoria - Copyright (c) Gustav

/** @file
Classes for input handling.
 */

#ifndef EUPHORIA_INPUT_ACTIVERANGE_H_
#define EUPHORIA_INPUT_ACTIVERANGE_H_

#include "euphoria/input-active.h"

namespace input {

class InputAction;
class Bind;

/** A input active range.
 */
class ActiveRange : public InputActive {
 public:
  /** Constructor.
  @param action the action this active belongs to
   */
  ActiveRange(InputAction* action, Bind* range);

  void update(float dt);

 private:
  Bind* range_;
};

}  // namespace input

#endif  // EUPHORIA_INPUT_ACTIVERANGE_H_
