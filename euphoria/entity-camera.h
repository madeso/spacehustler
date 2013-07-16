// Euphoria - Copyright (c) Gustav

/** @file
Entity Camera System.
 */

#ifndef EUPHORIA_ENTITY_CAMERA_H_
#define EUPHORIA_ENTITY_CAMERA_H_

#include "euphoria/entity.h"

#include "euphoria/camera.h"

/** Add the camera system to the system container.
@param container the container to add the rendering system to.
@param camera the camera to use.
@ingroup systems
 */
void Entity_AddCamera(SystemContainer* container, Camera* camera);

#endif  // EUPHORIA_ENTITY_CAMERA_H_
