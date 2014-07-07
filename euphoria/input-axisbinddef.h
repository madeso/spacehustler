// Euphoria - Copyright (c) Gustav

/** @file
Classes for input handling.
 */

#ifndef EUPHORIA_INPUT_AXISBINDDEF_H_
#define EUPHORIA_INPUT_AXISBINDDEF_H_

#include <memory>

namespace input {

class AxisBind;
class Action;

class AxisBindDef {
 public:
  explicit AxisBindDef(InputAction* action);
  std::shared_ptr<AxisBind> create();

 private:
  InputAction* action_;
};

}  // namespace input

#endif  // EUPHORIA_INPUT_AXISBINDDEF_H_
