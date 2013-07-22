// Euphoria - Copyright (c) Gustav

/** @file
Entity rendering System.
 */

#ifndef EUPHORIA_ENTITY_RENDERING_H_
#define EUPHORIA_ENTITY_RENDERING_H_

#include "euphoria/systems.h"

/** Add the rendering system to the system creator list.
@param sc the list.
@ingroup systems
 */
void Entity_AddRendering(SystemCreatorList* sc);

#endif  // EUPHORIA_ENTITY_RENDERING_H_
