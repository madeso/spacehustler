// Euphoria - Copyright (c) Gustav

/** @file
Classes for input handling.
 */

#ifndef EUPHORIA_INPUT_BINDLIST_H_
#define EUPHORIA_INPUT_BINDLIST_H_

#include <vector>
class Table;

namespace input {

class ActiveRangeToAxis;
class ActiveRange;
class ActiveAxis;

/** A list of binds. Belongs to the player.
 */
class BindList {
 public:
  /** Update the table with the actions.
   */
  void UpdateTable(Table* table);

 private:
  std::vector<ActiveRangeToAxis*> rangeToAxes_;
  std::vector<ActiveRange*> ranges_;
  std::vector<ActiveAxis*> axes_;
};

}  // namespace input

#endif  // EUPHORIA_INPUT_BINDLIST_H_
