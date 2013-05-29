// Euphoria - Copyright (c) Gustav

#ifndef EUPHORIA_MESH_H_
#define EUPHORIA_MESH_H_

#include <vector>

#include "boost/shared_ptr.hpp"

#include "euphoria/opengl.h"
#include "euphoria/shader.h"
#include "euphoria/texture.h"
#include "euphoria/camera.h"

class Mesh {
  public:
    Mesh();

    void addPoint(GLfloat x, GLfloat y, GLfloat z, GLfloat u, GLfloat v);
    std::vector<GLfloat> vertices;

    int points;
};

class Vao {
  public:
    Vao();
    ~Vao();

    GLuint get() const;
    void bind() const;
    void unbind();

  private:
    GLuint object;
};

class Vbo {
  public:
    Vbo();
    ~Vbo();

    GLuint get() const;
    void bind() const;
    void unbind();
  private:
    GLuint object;
};

class CompiledMesh {
  public:
    ~CompiledMesh();

    CompiledMesh(const Mesh& mesh, boost::shared_ptr<Program> program,
                 boost::shared_ptr<Texture> tex);
    void render(const Camera& camera, const mat44& model);

  private:
    Vao vao;
    Vbo vbo;

    const boost::shared_ptr<Program> program;
    boost::shared_ptr<Texture> texture;

    int points;
};

#endif  // EUPHORIA_MESH_H_
