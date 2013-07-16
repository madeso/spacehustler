// Euphoria - Copyright (c) Gustav

/** @file
World related code.
 */

#ifndef EUPHORIA_WORLD_H_
#define EUPHORIA_WORLD_H_

#include <memory>
#include <vector>
#include <string>

#include "euphoria/mesh.h"
#include "euphoria/math.h"
#include "euphoria/camera.h"
#include "euphoria/debugrenderer.h"
#include "euphoria/texturecache.h"
#include "euphoria/shadercache.h"

/** A mesh placed in a world.
@see CompiledMesh
@see World
 */
class Instance {
  public:
    /** Constructs a new instance.
    @param mesh the mesh to be rendered.
    @param transform the initial transformation of the mesh.
     */
    Instance(std::shared_ptr<CompiledMesh> mesh, const mat44& transform);

    /** Render this mesh. Should really only be called by World::render()
    @see World::render()
    @param camera the camera to render through.
     */
    void render(const Camera& camera);

    /** The mesh to be rendered.
     */
    std::shared_ptr<CompiledMesh> mesh;

    /** The transform of the instance.
     */
    mat44 transform;
};

/** List of instances that can be rendered.
 Ideally this would render the visible instances, but currently everything is always rendered.
 @todo implement frustum culling.
 */
class World {
  public:
    /** Constructs a new empty world.
    @param filename the file to load from
    @param texturecache the texture cache to load from
    @param shadercache the shader cache to load from
     */
    World(const std::string& filename, TextureCache* texturecache,
          ShaderCache* shadercache);

    /** Add a instance to the world.
    @param instance the instance to add.
     */
    void add(std::shared_ptr<Instance> instance);

    /** Render the world through a camera.
    @param camera the camera.
     */
    void render(const Camera& camera);

    /** Get the debug render.
    @returns the debug renderer.
     */
    DebugRenderer& debug();

  private:
    std::vector<std::shared_ptr<Instance>> instances;
    Mesh collisionmesh;
    DebugRenderer debugrenderer;
};

#endif  // EUPHORIA_WORLD_H_
