// Euphoria - Copyright (c) Gustav

/** @file
Classes for input handling.
 */

#ifndef EUPHORIA_INPUT_BIND_H_
#define EUPHORIA_INPUT_BIND_H_

namespace input {

class Action;

/** A bind.
 */
class Bind {
 public:
  explicit Bind(Action* action);

  float value() const;
  void set_value(float v);

 private:
  Action* action_;
  float value_;
};

}  // namespace input

#endif  // EUPHORIA_INPUT_BIND_H_
