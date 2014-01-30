// Euphoria - Copyright (c) Gustav

/** @file
Classes for input handling.
 */

#ifndef EUPHORIA_INPUT_CONNECTEDUNITS_H_
#define EUPHORIA_INPUT_CONNECTEDUNITS_H_

#include <vector>
#include <memory>

class Table;

namespace input {

class ActiveUnit;

/** Contains a list of active units.
 */
class ConnectedUnits {
 public:
  /** Constructor.
   */
  ConnectedUnits();

  /** Updates all connected units.
  @param table the table to update
   */
  void UpdateTable(Table* table);

  /** Add a unit.
  @param unit the unit to add
   */
  void Add(std::shared_ptr<ActiveUnit> unit);

  /** Are there any connected units?
  @returns true if there are no active units, false if not
   */
  bool IsEmpty() const;

 private:
  std::vector<std::shared_ptr<ActiveUnit>> units_;
};

}  // namespace input

#endif  // EUPHORIA_INPUT_CONNECTEDUNITS_H_
