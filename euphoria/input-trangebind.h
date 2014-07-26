// Euphoria - Copyright (c) Gustav

/** @file
Classes for input handling.
 */

#ifndef EUPHORIA_INPUT_TRANGEBIND_H_
#define EUPHORIA_INPUT_TRANGEBIND_H_

#include <memory>
#include <cassert>

#include "euphoria/input-bind.h"

namespace input {
template <typename T>
class TRangeBind {
 public:
  TRangeBind(T button, std::shared_ptr<Bind> bind)
      : button_(button), bind_(bind) {
    assert(this);
    assert(bind);
  }

  const T type() const {
    assert(this);
    return button_;
  }

  std::shared_ptr<Bind> bind() {
    assert(this);
    assert(bind_);
    return bind_;
  }

 private:
  T button_;
  std::shared_ptr<Bind> bind_;
};

}  // namespace input

#endif  // EUPHORIA_INPUT_TRANGEBIND_H_
