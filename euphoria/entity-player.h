// Euphoria - Copyright (c) Gustav

/** @file
Entity Camera System.
 */

#ifndef EUPHORIA_ENTITY_PLAYER_H_
#define EUPHORIA_ENTITY_PLAYER_H_

#include "euphoria/systems.h"

namespace euphoria {

/** Add the player system to the system creator list.
@param sc the list.
@ingroup systems
 */
void EntityAddPlayer(SystemCreatorList* sc);

}  // namespace euphoria

#endif  // EUPHORIA_ENTITY_PLAYER_H_
