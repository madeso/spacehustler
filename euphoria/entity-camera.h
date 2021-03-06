// Euphoria - Copyright (c) Gustav

/** @file
Entity Camera System.
 */

#ifndef EUPHORIA_ENTITY_CAMERA_H_
#define EUPHORIA_ENTITY_CAMERA_H_

#include "euphoria/systems.h"

namespace euphoria {

/** Add the camera system to the system creator list.
@param sc the list.
@ingroup systems
 */
void EntityAddCamera(SystemCreatorList* sc);

}  // namespace euphoria

#endif  // EUPHORIA_ENTITY_CAMERA_H_
