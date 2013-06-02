// Euphoria - Copyright (c) Gustav

#ifndef EUPHORIA_MESH_H_
#define EUPHORIA_MESH_H_

#include <vector>
#include <string>

#include "boost/shared_ptr.hpp"

#include "euphoria/opengl.h"
#include "euphoria/shader.h"
#include "euphoria/texture.h"
#include "euphoria/camera.h"

/** Represents a mesh.
 Usually loaded from disc but can also be dynamically generated.
 @see CompiledMesh
 */
class Mesh {
  public:
    /** Constructs a empty mesh.
     */
    Mesh();

    /** Add a point to the mesh.
     @param x the X coordinate.
     @param y the Y coordinate.
     @param z the Z coordinate.
     @param u the U texture coordinate.
     @param v the V texture coordinate.
     */
    void addPoint(GLfloat x, GLfloat y, GLfloat z, GLfloat u, GLfloat v);

    /** The vertices.
     */
    std::vector<GLfloat> vertices;

    /** THe number of points.
    @todo replace with a better structure.
     */
    int points;
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
@return the created sphere.
 */
Mesh CreateSphere(float size);

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

/** A RAII class for a OpenGL Vertex Buffer Object.
 */
class Vbo {
  public:
    /** Construct the VBO.
     */
    Vbo();

    /** Destruct the VBO.
     */
    ~Vbo();

    /** Get the OpenGL VBO object id.
    @return the object id.
     */
    GLuint get() const;

    /** Utility function for binding this VBO.
     */
    void bind() const;

    /** Utility function for unbinding this VBO.
     */
    void unbind();
  private:
    GLuint object;
};

/** Compiled mesh ready for rendering.
@see Mesh
@see Instance
 */
class CompiledMesh {
  public:
    /** Compiles a mesh.
    @param mesh the Mesh to compile.
    @param program the shader program to use.
    @param texture the texture to use.
     */
    CompiledMesh(const Mesh& mesh, boost::shared_ptr<Program> program,
                 boost::shared_ptr<Texture> texture);

    /** Destructs the compiled mesh.
     */
    ~CompiledMesh();

    /** Render the mesh.
    @param camera through the camera.
    @param model the model matrix
     */
    void render(const Camera& camera, const mat44& model);

  private:
    Vao vao;
    Vbo vbo;

    const boost::shared_ptr<Program> program;
    boost::shared_ptr<Texture> texture;

    int points;
};

#endif  // EUPHORIA_MESH_H_
