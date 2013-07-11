// Euphoria - Copyright (c) Gustav

/** @file
Entity rendering System.
 */

#ifndef EUPHORIA_ENTITY_RENDERING_H_
#define EUPHORIA_ENTITY_RENDERING_H_

#include "euphoria/entity.h"

#include "euphoria/world.h"
#include "euphoria/texturecache.h"
#include "euphoria/shadercache.h"

/** Add the rendering system to the system container.
@param container the container to add the rendering system to.
@param world the world to add the entity objects systems.
@param tc the texture cache.
@param sc the shader cache.
@ingroup systems
 */
void Entity_AddRendering(SystemContainer* container, World* world,
                         TextureCache* tc, ShaderCache* sc);

#endif  // EUPHORIA_ENTITY_RENDERING_H_
