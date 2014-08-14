// Euphoria - Copyright (c) Gustav

/** @file
Classes for input handling.
 */

#ifndef EUPHORIA_INPUT_BINDDEF_H_
#define EUPHORIA_INPUT_BINDDEF_H_

#include <string>
#include <cassert>

#include "json/json.h"

namespace euphoria {

namespace input {

/** Definition of a bind.
 */

template <typename Type>
class BindDef {
 public:
  BindDef(const std::string& id, const Type type, const Json::Value& d)
      : id_(id),
        type_(type),
        invert_(d.get("invert", false).asBool()),
        scale_(d.get("scale", 1.0f).asFloat()) {
    assert(this);
  }
  const std::string& id() const {
    assert(this);
    return id_;
  }
  const Type type() const {
    assert(this);
    return type_;
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
  std::string id_;
  Type type_;
  bool invert_;
  float scale_;
};

}  // namespace input

}  // namespace euphoria

#endif  // EUPHORIA_INPUT_BINDDEF_H_
