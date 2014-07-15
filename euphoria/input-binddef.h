// Euphoria - Copyright (c) Gustav

/** @file
Classes for input handling.
 */

#ifndef EUPHORIA_INPUT_BINDDEF_H_
#define EUPHORIA_INPUT_BINDDEF_H_

#include <string>
#include <cassert>

namespace input {

/** Definition of a bind.
 */

template <typename Type>
class BindDef {
 public:
   BindDef(const std::string& id, const Type type)
     : id_(id), type_(type) {assert(this);}
   const std::string& id() const {
     assert(this);
     return id_;
   }
   const Type type() const {
     assert(this);
     return type_;
   }
 private:
   std::string id_;
   Type type_;
};

}  // namespace input

#endif  // EUPHORIA_INPUT_BINDDEF_H_
