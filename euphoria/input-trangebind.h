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
  TRangeBind(T button, std::shared_ptr<Bind> bind, bool invert, float scale)
      : button_(button), bind_(bind), invert_(invert), scale_(scale) {
    assert(this);
    assert(bind);
  }

  const T GetType() const {
    assert(this);
    return button_;
  }

  std::shared_ptr<Bind> bind() {
    assert(this);
    assert(bind_);
    return bind_;
  }

  bool invert() const {
    assert(this);
    return invert_;
  }

  float scale() const {
    assert(this);
    return scale_;
  }

 private:
  T button_;
  std::shared_ptr<Bind> bind_;

  bool invert_;
  float scale_;
};

}  // namespace input

#endif  // EUPHORIA_INPUT_TRANGEBIND_H_
