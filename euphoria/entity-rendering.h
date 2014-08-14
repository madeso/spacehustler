// Euphoria - Copyright (c) Gustav

/** @file
Entity rendering System.
 */

#ifndef EUPHORIA_ENTITY_RENDERING_H_
#define EUPHORIA_ENTITY_RENDERING_H_

#include "euphoria/systems.h"

namespace euphoria {

/** Add the rendering system to the system creator list.
@param sc the list.
@ingroup systems
 */
void EntityAddRendering(SystemCreatorList* sc);

}  // namespace euphoria

#endif  // EUPHORIA_ENTITY_RENDERING_H_
