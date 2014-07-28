// Euphoria - Copyright (c) Gustav

/** @file
Classes for input handling.
 */

#ifndef EUPHORIA_INPUT_BIND_H_
#define EUPHORIA_INPUT_BIND_H_

#include <string>

namespace input {

class InputAction;
class GlobalToggle;

enum class BindType {
  Range,
  Axis
};

std::string ToString(const BindType& bt);

template <typename Stream>
Stream& operator<<(Stream& ss, const BindType& bt) {
  ss << ToString(bt);
  return ss;
}

/** A bind.
 */
class Bind {
 public:
  Bind(InputAction* action, BindType type);
  ~Bind();

  float value() const;
  void set_value(float v);

  BindType type() const;
  InputAction* action();

 private:
  InputAction* action_;
  BindType type_;
  GlobalToggle* toggle_;
  float value_;
};

}  // namespace input

#endif  // EUPHORIA_INPUT_BIND_H_
