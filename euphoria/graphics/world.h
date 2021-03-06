// Euphoria - Copyright (c) Gustav

/** @file
World related code.
 */

#ifndef EUPHORIA_GRAPHICS_WORLD_H_
#define EUPHORIA_GRAPHICS_WORLD_H_

#include <memory>
#include <vector>
#include <string>

#include "euphoria/graphics/mesh.h"
#include "euphoria/math.h"
#include "euphoria/graphics/camera.h"
#include "euphoria/graphics/debugrenderer.h"
#include "euphoria/graphics/texturecache.h"
#include "euphoria/graphics/shadercache.h"

namespace euphoria {

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
  Instance(std::shared_ptr<CompiledMesh> mesh, const Mat44& transform);

  /** Render this mesh. Should really only be called by World::render()
  @see World::render()
  @param camera the camera to render through.
   */
  void Render(const Camera& camera);

  const Mat44& transform() const;
  void set_transform(const Mat44& transform);

 private:
  /** The mesh to be rendered.
   */
  std::shared_ptr<CompiledMesh> mesh_;

  /** The transform of the instance.
   */
  Mat44 transform_;
};

/** List of instances that can be rendered.
 Ideally this would render the visible instances, but currently everything is
 always rendered.
 @todo implement frustum culling.
 */
class World {
 public:
  /** Constructs a new empty world.
  @param filename the file to load from
  @param texturecache the texture cache to load from
  @param shadercache the shader cache to load from
  @param settings the settings to use
   */
  World(const std::string& filename, TextureCache* texturecache,
        ShaderCache* shadercache, const Settings& settings);

  /** Add a instance to the world.
  @param instance the instance to add.
   */
  void Add(std::shared_ptr<Instance> instance);

  /** Render the world through a camera.
  @param camera the camera.
   */
  void Render(const Camera& camera);

  /** Get the debug render.
  @returns the debug renderer.
   */
  DebugRenderer& debug_renderer();

  /** Gets the collision mesh.
  @returns the collision mesh
   */
  const Mesh& collisionmesh() const;

 private:
  bool debug_transforms_;
  std::vector<std::shared_ptr<Instance>> instances_;
  Mesh collisionmesh_;
  DebugRenderer debug_renderer_;
};

}  // namespace euphoria

#endif  // EUPHORIA_GRAPHICS_WORLD_H_
