// Euphoria - Copyright (c) Gustav

/** @file
Classes for input handling.
 */

#ifndef EUPHORIA_INPUT_TAXISBIND_H_
#define EUPHORIA_INPUT_TAXISBIND_H_

#include <memory>
#include <cassert>

#include "euphoria/input-bind.h"

namespace euphoria {

namespace input {

template <typename T>
class TAxisBind {
 public:
  TAxisBind(T axis, std::shared_ptr<Bind> bind, bool invert, float scale)
      : axis_(axis), bind_(bind), invert_(invert), scale_(scale) {
    assert(this);
    assert(bind);
  }

  const T GetType() const {
    assert(this);
    return axis_;
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
  T axis_;
  std::shared_ptr<Bind> bind_;
  bool invert_;
  float scale_;
};

}  // namespace input

}  // namespace euphoria

#endif  // EUPHORIA_INPUT_TAXISBIND_H_
