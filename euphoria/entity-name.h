// Euphoria - Copyright (c) Gustav

/** @file
Entity Name System.
 */

#ifndef EUPHORIA_ENTITY_NAME_H_
#define EUPHORIA_ENTITY_NAME_H_

#include "euphoria/systems.h"

namespace euphoria {

/** Add the name system to the system creator list.
@param sc the list.
@ingroup systems
 */
void EntityAddName(SystemCreatorList* sc);

}  // namespace euphoria

#endif  // EUPHORIA_ENTITY_NAME_H_
