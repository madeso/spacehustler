// Euphoria - Copyright (c) Gustav

/** @file
Classes for input handling.
 */

#ifndef EUPHORIA_INPUT_PLAYER_H_
#define EUPHORIA_INPUT_PLAYER_H_

#include <memory>

#include "euphoria/input-connectedunits.h"

namespace euphoria {

class Table;

namespace input {

/** Represents a player.
The idea behind decoupling the active units and the player is that the unit
could be disconnected and swapped but the player should remain.
*/
class Player {
 public:
  /** Constructor.
   */
  Player();

  /** Updates all connected units.
  @param table the table to update
  */
  void UpdateTable(Table* table);

  void Update(float dt);

  /** Sets the connected units.
  @param units the units
  */
  void set_units(std::shared_ptr<ConnectedUnits> units);

 private:
  std::shared_ptr<ConnectedUnits> units_;
};

}  // namespace input

}  // namespace euphoria

#endif  // EUPHORIA_INPUT_PLAYER_H_
