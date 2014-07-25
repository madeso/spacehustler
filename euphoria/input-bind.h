// Euphoria - Copyright (c) Gustav

/** @file
Classes for input handling.
 */

#ifndef EUPHORIA_INPUT_BIND_H_
#define EUPHORIA_INPUT_BIND_H_

namespace input {

class InputAction;
class GlobalToggle;

namespace BindType {
enum Type {
  Range,
  Axis
};
}

/** A bind.
 */
class Bind {
 public:
  Bind(InputAction* action, BindType::Type type);
  ~Bind();

  float value() const;
  void set_value(float v);

  BindType::Type type() const;
  InputAction* action();

 private:
  InputAction* action_;
  BindType::Type type_;
  GlobalToggle* toggle_;
  float value_;
};

}  // namespace input

#endif  // EUPHORIA_INPUT_BIND_H_
