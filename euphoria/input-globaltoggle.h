// Euphoria - Copyright (c) Gustav

/** @file
Classes for input handling.
 */

#ifndef EUPHORIA_INPUT_GLOBALTOGGLE_H_
#define EUPHORIA_INPUT_GLOBALTOGGLE_H_

#include <memory>
#include <vector>

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

  bool state() const;

 private:
  std::shared_ptr<InputAction> action_;
  bool state_;
  bool last_down_;
  Bind* current_state_holder_;
  std::vector<Bind*> binds_;
};

}  // namespace input

#endif  // EUPHORIA_INPUT_GLOBALTOGGLE_H_
