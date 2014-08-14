// Euphoria - Copyright (c) Gustav

/** @file
Script System.
 */

#ifndef EUPHORIA_ENTITY_SCRIPT_H_
#define EUPHORIA_ENTITY_SCRIPT_H_

#include "euphoria/systems.h"

namespace euphoria {

/** Add the scripting system to the system creator list.
@param sc the list.
@ingroup systems
 */
void EntityAddScript(SystemCreatorList* sc);

}  // namespace euphoria

#endif  // EUPHORIA_ENTITY_SCRIPT_H_
