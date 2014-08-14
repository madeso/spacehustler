// Euphoria - Copyright (c) Gustav

/** @file
Entity Physics System.
 */

#ifndef EUPHORIA_ENTITY_PHYSICS_H_
#define EUPHORIA_ENTITY_PHYSICS_H_

#include "euphoria/systems.h"

namespace euphoria {

/** Add the physics system to the system creator list.
@param sc the list.
@ingroup systems
 */
void EntityAddPhysics(SystemCreatorList* sc);

}  // namespace euphoria

#endif  // EUPHORIA_ENTITY_PHYSICS_H_
