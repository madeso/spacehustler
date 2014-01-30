// Euphoria - Copyright (c) Gustav

/** @file
Classes for input handling.
 */

#ifndef EUPHORIA_INPUT_BIND_H_
#define EUPHORIA_INPUT_BIND_H_

#include <memory>
#include <cassert>

namespace input {
class InputAction;

template <typename Type>
class Bind {
 public:
  Bind(Type type, std::shared_ptr<InputAction> action)
      : type_(type), action_(action) {
    assert(this);
  }

  Type type() const {
    assert(this);
    return type_;
  }

  std::shared_ptr<InputAction> action() const {
    assert(this);
    return action_;
  }

 private:
  Type type_;
  std::shared_ptr<InputAction> action_;
};

}  // namespace input

#endif  // EUPHORIA_INPUT_BIND_H_
