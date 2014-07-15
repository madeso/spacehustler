// Euphoria - Copyright (c) Gustav

/** @file
Classes for input handling.
 */

#ifndef EUPHORIA_INPUT_BINDLIST_H_
#define EUPHORIA_INPUT_BINDLIST_H_

#include <vector>

namespace input {

class ActiveRangeToAxis;
class ActiveRange;
class ActiveAxis;

/** Global list of binds.
 */
class BindList {
 public:
 private:
  std::vector<ActiveRangeToAxis*> rangeToAxes_;
  std::vector<ActiveRange*> ranges_;
  std::vector<ActiveAxis*> axes_;
};

}  // namespace input

#endif  // EUPHORIA_INPUT_BINDLIST_H_
