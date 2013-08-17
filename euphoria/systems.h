// Euphoria - Copyright (c) Gustav

/** @file
Contains systems loading code.
 */

#ifndef EUPHORIA_SYSTEMS_H_
#define EUPHORIA_SYSTEMS_H_

#include <string>
#include <functional>
#include <map>

namespace Json {
  class Value;
}  // namespace Json

class SystemContainer;
class World;
class TextureCache;
class ShaderCache;
class Camera;
class Lua;

/** Arguments for creating a system.
 */
class CreateSystemArg {
  public:
    /** Constructor.
    @param acontainer where to put the systems
    @param aworld the world
    @param atexturecache the texture cache
    @param ashadercache the shader cache
    @param acamera the camera
    @param ascript the lua engine
     */
    CreateSystemArg(
      SystemContainer* acontainer,
      World* aworld,
      TextureCache* atexturecache,
      ShaderCache* ashadercache,
      Camera* acamera,
      Lua* ascript);

    /** The system container.
     */
    SystemContainer* container;

    /** The world.
     */
    World* world;

    /** The texture cache.
     */
    TextureCache* texturecache;

    /** The shader cache.
     */
    ShaderCache* shadercache;

    /** The camera.
     */
    Camera* camera;

    /** The main lua engine.
     */
    Lua* script;
};

/** Callback function for creating systems.
First argument is the general arguments.
Second argument is the per system arguments defined when creating the object.
 */
typedef std::function<void(CreateSystemArg, Json::Value)> Callback;

/** List of system creators
 */
class SystemCreatorList {
  public:
    /** Constructor.
     */
    SystemCreatorList();

    /** Add a system creator.
    @param name the name of the system
    @param callback the callback to use.
     */
    void Add(const std::string& name, Callback callback);

    /** Get a previously added callback.
    @param name the name of the system
    @throws exception if the callback couldn't be found
    @returns the callback if found
     */
    Callback Get(const std::string& name) const;
  private:
    std::map<std::string, Callback> creators_;
};

/** Loads system definitions from a file.
@param filename the file to load from
@param arg the arguments to use.
 */
void LoadSystems(const std::string& filename, CreateSystemArg arg);

#endif  // EUPHORIA_SYSTEMS_H_
