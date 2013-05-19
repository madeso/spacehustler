#ifndef EUPHORIA_MESH_H_
#define EUPHORIA_MESH_H_

#include <vector>

#include "opengl.h"
#include "shader.h"

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

    CompiledMesh(const Mesh& mesh, const Program& p);
    void render();

  private:
    Vao vao;
    Vbo vbo;

    int points;
};

#endif  // EUPHORIA_MESH_H_
