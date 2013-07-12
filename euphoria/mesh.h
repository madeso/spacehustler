// Euphoria - Copyright (c) Gustav

/** @file
Mesh related code.
 */

#ifndef EUPHORIA_MESH_H_
#define EUPHORIA_MESH_H_

#include <vector>
#include <string>

#include "boost/shared_ptr.hpp"

#include "euphoria/opengl.h"
#include "euphoria/shader.h"
#include "euphoria/texture.h"
#include "euphoria/camera.h"
#include "euphoria/texturecache.h"
#include "euphoria/shadercache.h"

namespace internal {

  /** Triangle soup with a single material. Part of a Mesh.
   */
  class MeshPart {
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
      void addPoint(GLfloat x, GLfloat y, GLfloat z, GLfloat u, GLfloat v);

      /** Add a face to the mesh.
      @param a the first index.
      @param b the second index;
      @param c the third index.
       */
      void addFace(unsigned int a, unsigned int b, unsigned int c);

      /** The vertices.
       */
      std::vector<GLfloat> vertices;

      /** The faces.
       */
      std::vector<GLushort> faces;

      /** THe number of points.
       */
      int points;

      /** The material index.
       */
      unsigned int material;
  };

  /** Represents a material.
   */
  class Material {
    public:
      /** Construct a Material.
       */
      Material();

      /** The path to the texture.
       */
      std::string texture;

      /** The wrapping for S.
       */
      Texture::WrapMode wraps;

      /** The wrapping for T.
       */
      Texture::WrapMode wrapt;
  };

}  // namespace internal

/** Represents a mesh.
 Usually loaded from disc but can also be dynamically generated.
 @see CompiledMesh
 */
class Mesh {
  public:
    /** Constructs a empty mesh.
     */
    Mesh();

    /** The parts.
     */
    std::vector<internal::MeshPart> parts;

    /** The materials.
    This is actually the textures, but the whole material class isn't implemented yet.
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
  /** A RAII class for a OpenGl Vertex Array Object.
   */
  class Vao {
    public:
      /** Construct the VAO.
       */
      Vao();

      /** Destruct the VAO.
       */
      ~Vao();

      /** Get the OpenGL VAO object id.
      @return the object id.
       */
      GLuint get() const;

      /** Utility function for binding this VAO.
       */
      void bind() const;

      /** Utility function for unbinding this VAO.
       */
      void unbind();

    private:
      GLuint object;
  };

  /** A RAII class for a OpenGL Buffer Object.
   */
  class BufferObject {
    protected:
      /** Construct the Buffer.
       */
      BufferObject();

    public:
      /** Destruct the Buffer.
       */
      ~BufferObject();

      /** Get the OpenGL Buffer object id.
      @return the object id.
       */
      GLuint get() const;

    private:
      GLuint object;
  };

  /** A RAII class for a OpenGL Array Buffer.
   */
  class ArrayBuffer : public BufferObject {
    public:
      /** Utility function for binding this Array Buffer.
       */
      void bind() const;

      /** Utility function for unbinding this Array Buffer.
       */
      void unbind();
  };

  /** A RAII class for a OpenGL Element Array Buffer.
   */
  class ElementArrayBuffer : public BufferObject {
    public:
      /** Utility function for binding this Element Array Buffer.
       */
      void bind() const;

      /** Utility function for unbinding this Element Array Buffer.
       */
      void unbind();
  };

  /** Compiled mesh part ready for rendering.
  @see CompiledMesh
   */
  class CompiledMeshPart {
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

      /** Render the mesh part.
      @param camera through the camera.
      @param model the model matrix
       */
      void render(const Camera& camera, const mat44& model);

    private:
      Vao vao;
      ArrayBuffer vbo;
      ElementArrayBuffer elements;
      GLsizei elementCount;

      const std::shared_ptr<Program> program;
      std::shared_ptr<Texture> texture;

      int points;
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
     */
    explicit CompiledMesh(const Mesh& mesh, TextureCache* texturecache,
                          ShaderCache* shadercache);

    /** Render the mesh.
      @param camera through the camera.
      @param model the model matrix
       */
    void render(const Camera& camera, const mat44& model);
  private:
    std::vector<std::shared_ptr<internal::CompiledMeshPart>> parts;
};

#endif  // EUPHORIA_MESH_H_
