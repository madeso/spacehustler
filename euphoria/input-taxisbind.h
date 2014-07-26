// Euphoria - Copyright (c) Gustav

/** @file
Classes for input handling.
 */

#ifndef EUPHORIA_INPUT_TAXISBIND_H_
#define EUPHORIA_INPUT_TAXISBIND_H_

#include <memory>
#include <cassert>

#include "euphoria/input-bind.h"

namespace input {

template <typename T>
class TAxisBind {
 public:
  TAxisBind(T axis, std::shared_ptr<Bind> bind) : axis_(axis), bind_(bind) {
    assert(this);
    assert(bind);
  }

  const T type() const {
    assert(this);
    return axis_;
  }

  std::shared_ptr<Bind> bind() {
    assert(this);
    assert(bind_);
    return bind_;
  }

 private:
  T axis_;
  std::shared_ptr<Bind> bind_;
};

}  // namespace input

#endif  // EUPHORIA_INPUT_TAXISBIND_H_
