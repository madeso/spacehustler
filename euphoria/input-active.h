// Euphoria - Copyright (c) Gustav

/** @file
Classes for input handling.
 */

#ifndef EUPHORIA_INPUT_ACTIVE_H_
#define EUPHORIA_INPUT_ACTIVE_H_

class Table;

namespace input {

class InputAction;

/** A input action.
 */
class InputActive {
 public:
  /** Constructor.
  @param action the action this active belongs to
   */
  explicit InputActive(InputAction* action);

  /** Get the current value of the input.
  @returns the state
   */
  float state() const;

  /** Set the value of the input.
  @param state the new value
   */
  void set_state(float state);

  /** Gets the action this active belongs to.
  @returns the action this active belongs to
   */
  const InputAction& action() const;

 private:
  InputAction* action_;
  float state_;
};

}  // namespace input

#endif  // EUPHORIA_INPUT_ACTIVE_H_
