// Euphoria - Copyright (c) Gustav

#include "euphoria/systems.h"

#include <string>

#include "euphoria/entity-rendering.h"
#include "euphoria/entity-physics.h"
#include "euphoria/entity-camera.h"

/// @todo use the filename
void LoadSystems(const std::string& filename, SystemContainer* container,
                 World* world, TextureCache* texturecache,
                 ShaderCache* shadercache, Camera* camera) {
  Entity_AddRendering(container, world, texturecache, shadercache);
  Entity_AddPhysics(container, world);
  Entity_AddCamera(container, camera);
}
