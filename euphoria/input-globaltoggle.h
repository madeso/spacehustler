// Euphoria - Copyright (c) Gustav

/** @file
Classes for input handling.
 */

#ifndef EUPHORIA_INPUT_GLOBALTOGGLE_H_
#define EUPHORIA_INPUT_GLOBALTOGGLE_H_

#include <memory>

namespace input {

class InputAction;
  class Bind;

/** Represents the accumulated input to be sent to the game logic.
 */
class GlobalToggle {
 public:
  explicit GlobalToggle(std::shared_ptr<InputAction> action);
  ~GlobalToggle();
  
  void Update();
  void Add(Bind* bind);
  void Remove(Bind* bind);
 private:
  std::shared_ptr<InputAction> action_;
  float state_;
};

}  // namespace input

#endif  // EUPHORIA_INPUT_GLOBALTOGGLE_H_
