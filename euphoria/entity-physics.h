// Euphoria - Copyright (c) Gustav

/** @file
Entity Physics System.
 */

#ifndef EUPHORIA_ENTITY_PHYSICS_H_
#define EUPHORIA_ENTITY_PHYSICS_H_

#include "euphoria/entity.h"
#include "euphoria/world.h"

/** Add the physics system to the system container.
@param container the container to add the physics system to
@param world the world containing the collision geometry
@ingroup systems
 */
void Entity_AddPhysics(SystemContainer* container, World* world);

#endif  // EUPHORIA_ENTITY_PHYSICS_H_
