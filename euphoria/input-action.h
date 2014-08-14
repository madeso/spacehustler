// Euphoria - Copyright (c) Gustav

/** @file
Classes for input handling.
 */

#ifndef EUPHORIA_INPUT_ACTION_H_
#define EUPHORIA_INPUT_ACTION_H_

#include <string>

#include "euphoria/input-range.h"

namespace euphoria {

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
  @param global true if this is marked as a global
   */
  InputAction(const std::string& name, const std::string& scriptvarname,
              Range range, bool global);
  ~InputAction();

  /** Get the name of the script var.
   */
  const std::string& scriptvarname() const;

  /** Gets the range of this input.
  @returns the range of this input
   */
  const Range range() const;

  const std::string& name() const;

  bool global() const;
  GlobalToggle* toggle();

  void Setup(GlobalToggle* toggle);
  void ClearToggle(GlobalToggle* toggle);

 private:
  std::string name_;
  std::string scriptvarname_;
  Range range_;
  bool global_;
  GlobalToggle* toggle_;
};

}  // namespace input

}  // namespace euphoria

#endif  // EUPHORIA_INPUT_ACTION_H_
