// Euphoria - Copyright (c) Gustav

/** @file
Contains systems loading code.
 */

#ifndef EUPHORIA_SYSTEMS_H_
#define EUPHORIA_SYSTEMS_H_

#include <string>

class SystemContainer;
class World;
class TextureCache;
class ShaderCache;
class Camera;

/** Loads system definitions from a file.
@param filename the file to load from
@param container where to put the systems
@param world the world
@param texturecache the texture cache
@param shadercache the shader cache
@param camera the camera
 */
void LoadSystems(const std::string& filename, SystemContainer* container,
                 World* world, TextureCache* texturecache,
                 ShaderCache* shadercache, Camera* camera);

#endif  // EUPHORIA_SYSTEMS_H_
