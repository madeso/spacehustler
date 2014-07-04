// Euphoria - Copyright (c) Gustav

/** @file
Classes for input handling.
 */

#ifndef EUPHORIA_INPUT_RANGEBINDDEF_H_
#define EUPHORIA_INPUT_RANGEBINDDEF_H_

#include <memory>

namespace input {

class RangeBind;
class Action;

/** The range bind def.
 */
class RangeBindDef {
 public:
  explicit RangeBindDef(Action* action);
  std::shared_ptr<RangeBind> create();

 private:
  Action* action_;
};

}  // namespace input

#endif  // EUPHORIA_INPUT_RANGEBINDDEF_H_
