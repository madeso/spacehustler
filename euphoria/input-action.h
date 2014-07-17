// Euphoria - Copyright (c) Gustav

/** @file
Classes for input handling.
 */

#ifndef EUPHORIA_INPUT_ACTION_H_
#define EUPHORIA_INPUT_ACTION_H_

#include <string>
#include <vector>

#include "euphoria/input-range.h"

class Table;

namespace input {

  class GlobalToggle;
  
/** A input action.
 */
class InputAction {
 public:
  /** Constructor.
  @param scriptvarname the name of the variable in the script.
  @param range the range of valid values for the scriptvar
   */
  InputAction(const std::string& scriptvarname, Range::Type range);

  /** Get the name of the script var.
   */
  const std::string& scriptvarname() const;

  /** Gets the range of this input.
  @returns the range of this input
   */
  const Range::Type range() const;
  
  void Add(GlobalToggle* toggle);
  void Remove(GlobalToggle* toggle);

 private:
  std::string scriptvarname_;
  Range::Type range_;
  std::vector<GlobalToggle* > toggles_; // transform into a unique vector instead?
};

}  // namespace input

#endif  // EUPHORIA_INPUT_ACTION_H_
