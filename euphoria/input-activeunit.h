// Euphoria - Copyright (c) Gustav

/** @file
Classes for input handling.
 */

#ifndef EUPHORIA_INPUT_ACTIVEUNIT_H_
#define EUPHORIA_INPUT_ACTIVEUNIT_H_

#include <vector>
#include <memory>

namespace input {

class InputAction;

/** Represents a active unit.
A perfect example is a connected joystick.
 */
class ActiveUnit {
 public:
  /** Destructor.
   */
  virtual ~ActiveUnit();

  /** Causes the active unit to rumble.
   */
  virtual void Rumble() = 0;

 protected:
  /** Constructor.
   */
  ActiveUnit();

  /** Add a action to be updated.
   */
  void Add(InputAction* action);

 private:
  std::vector<InputAction*> actions_;
};

}  // namespace input

#endif  // EUPHORIA_INPUT_ACTIVEUNIT_H_
