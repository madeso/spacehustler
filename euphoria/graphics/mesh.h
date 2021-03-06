// Euphoria - Copyright (c) Gustav

/** @file
Mesh related code.
 */

#ifndef EUPHORIA_GRAPHICS_MESH_H_
#define EUPHORIA_GRAPHICS_MESH_H_

#include <vector>
#include <string>

#include "euphoria/graphics/opengl.h"
#include "euphoria/graphics/shader.h"
#include "euphoria/graphics/texture.h"
#include "euphoria/graphics/camera.h"
#include "euphoria/graphics/texturecache.h"
#include "euphoria/graphics/shadercache.h"
#include "euphoria/graphics/openglraii.h"

namespace euphoria {

namespace internal {

/** Triangle soup with a single material. Part of a Mesh.
 */
struct MeshPart {
 public:
  /** Constructs a empty mesh.
   */
  MeshPart();

  /** Add a point to the mesh.
   @param x the X coordinate.
   @param y the Y coordinate.
   @param z the Z coordinate.
   @param u the U texture coordinate.
   @param v the V texture coordinate.
   */
  void AddPoint(GLfloat x, GLfloat y, GLfloat z, GLfloat u, GLfloat v);

  /** Add a face to the mesh.
  @param a the first index.
  @param b the second index;
  @param c the third index.
   */
  void AddFace(unsigned int a, unsigned int b, unsigned int c);

  /** Utility function for getting a vertex.
  @param p the index for the vertex to get
  @return the vertex
   */
  Vec3 GetVertex(unsigned int p) const;

  /** The vertices.
   */
  std::vector<GLfloat> vertices;

  /** The faces.
   */
  std::vector<GLushort> faces;

  /** The number of points.
   */
  unsigned int points;

  /** The material index.
   */
  unsigned int material;
};

/** Represents a material.
 */
struct Material {
 public:
  /** Construct a Material.
   */
  Material();

  /** The path to the texture.
   */
  std::string texture;

  /** The wrapping for S.
   */
  WrapMode wraps;

  /** The wrapping for T.
   */
  WrapMode wrapt;
};

}  // namespace internal

/** Represents a mesh.
 Usually loaded from disc but can also be dynamically generated.
 @see CompiledMesh
 */
struct Mesh {
 public:
  /** Constructs a empty mesh.
   */
  Mesh();

  /** The parts.
   */
  std::vector<internal::MeshPart> parts;

  /** The materials.
  This is actually the textures, but the whole material class isn't implemented
  yet.
  @todo update to a better material representation
   */
  std::vector<internal::Material> materials;
};

/** Load a mesh.
@param path the file path.
@return the loaded mesh
 */
Mesh LoadMesh(const std::string& path);

/** Create a cube.
@param size the size of the cube.
@return the created cube.
 */
Mesh CreateCube(float size);

/** Create a sphere.
@param size the radius of the sphere.
@param texture the path of the texture
@return the created sphere.
 */
Mesh CreateSphere(float size, const std::string& texture);

/** Create a box.
@param width the width of the box.
@param height the height of the box.
@param depth the depth of the box.
@return the created box.
 */
Mesh CreateBox(float width, float height, float depth);

namespace internal {
/** Compiled mesh part ready for rendering.
@see CompiledMesh
 */
class BasicCompiledMeshPart {
 public:
  /** Compiles a mesh part.
  @param mesh the MeshPart to compile.
  @param program the shader program to use.
  @param texture the texture to use.
   */
  BasicCompiledMeshPart(const MeshPart& mesh, std::shared_ptr<Program> program,
                        std::shared_ptr<Texture> texture);

  /** Destructs the compiled mesh part.
   */
  ~BasicCompiledMeshPart();

  /** Render the mesh part as seen through a camera.
  @param camera through the camera.
  @param model the model matrix
   */
  void Render(const Camera& camera, const Mat44& model);

  /** Render the mesh.
  Requires both the shader and the texture to be bound.
   */
  void Render();

 protected:
  void CreateBegin();
  void CreateVertexes(const std::vector<GLfloat>& vertices, bool reset);
  void CreateFaces(const std::vector<GLushort>& faces);
  void CreateEnd();

 protected:
  Vao vao_;
  ArrayBuffer vbo_;
  ElementArrayBuffer elements_;

  const std::shared_ptr<Program> program_;
  std::shared_ptr<Texture> texture_;
  GLsizei element_count_;
};

class CompiledMeshPart : public BasicCompiledMeshPart {
 public:
  /** Compiles a mesh part.
  @param mesh the MeshPart to compile.
  @param program the shader program to use.
  @param texture the texture to use.
   */
  CompiledMeshPart(const MeshPart& mesh, std::shared_ptr<Program> program,
                   std::shared_ptr<Texture> texture);

  /** Destructs the compiled mesh part.
   */
  ~CompiledMeshPart();
};

class DynamicMeshPart : public BasicCompiledMeshPart {
 public:
  DynamicMeshPart(const MeshPart& mesh, std::shared_ptr<Program> program,
                  std::shared_ptr<Texture> texture);
  ~DynamicMeshPart();

  void UpdateMesh();
  void SetVertex(unsigned int index, const Vec3 pos);
  void SetVertex(unsigned int index, Vec2 uv);

 private:
  std::vector<GLfloat> vertices_;
};

}  // namespace internal

/** Compiled mesh ready for rendering.
@see Mesh
@see Instance
 */
class CompiledMesh {
 public:
  /** Compiles a mesh.
  @param mesh the MeshPart to compile.
  @param texturecache the cache from where to get the textures from
  @param shadercache the cache from where to get the shaders from
  @param settings the settings to use
   */
  explicit CompiledMesh(const Mesh& mesh, TextureCache* texturecache,
                        ShaderCache* shadercache, const Settings& settings);

  /** Render the mesh.
    @param camera through the camera.
    @param model the model matrix
     */
  void Render(const Camera& camera, const Mat44& model);

 private:
  std::vector<std::shared_ptr<internal::CompiledMeshPart>> parts_;
};

}  // namespace euphoria

#endif  // EUPHORIA_GRAPHICS_MESH_H_
