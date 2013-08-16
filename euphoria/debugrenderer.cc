// Euphoria - Copyright (c) Gustav

#include "euphoria/debugrenderer.h"
#include <cassert>
#include <vector>

DebugRenderer::DebugRenderer(ShaderCache* shadercache)
  : linecount(0) {
  prog = shadercache->get("debuglines.js");
}

namespace {
  void Add(std::vector<GLfloat>* d, const vec3& p) {
    d->push_back(p[0]);
    d->push_back(p[1]);
    d->push_back(p[2]);
  }

  void Add(std::vector<GLfloat>* d, const Color& c) {
    d->push_back(c.r);
    d->push_back(c.g);
    d->push_back(c.b);
  }
}  // namespace

void DebugRenderer::line(const vec3& f, const vec3& t, const Color& c) {
  line(f, t, c, c);
}

void DebugRenderer::line(const vec3& f, const vec3& t, const Color& fc,
                         const Color& tc) {
  Add(&pending, f);
  Add(&pending, fc);
  Add(&pending, t);
  Add(&pending, tc);
  ++linecount;
}

void DebugRenderer::update() {
  vao.reset(new internal::Vao());
  vbo.reset(new internal::ArrayBuffer());
  points = pending;

  vao->bind();
  vbo->bind();

  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * points.size(), &points[0],
               GL_STATIC_DRAW);

  const GLsizei stride = 6 * sizeof(GLfloat);
  const GLvoid* coloroffset = reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat));
  const auto vert = prog->attrib("vert");
  const auto color = prog->attrib("color");
  glEnableVertexAttribArray(vert);
  glVertexAttribPointer(vert, 3, GL_FLOAT, GL_FALSE, stride, NULL);
  glEnableVertexAttribArray(color);
  glVertexAttribPointer(color, 3, GL_FLOAT, GL_TRUE, stride, coloroffset);

  vbo->unbind();
  vao->unbind();

  pending.clear();
}

void DebugRenderer::render(const Camera& camera) {
  if (linecount != 0) {
    update();

    prog->bind();

    prog->setUniform("camera", camera.view);
    prog->setUniform("projection", camera.GetProjection());

    vao->bind();
    vbo->bind();
    glDrawArrays(GL_LINES, 0, linecount * 2);
    vbo->unbind();
    vao->unbind();
    prog->unbind();

    linecount = 0;
  }
}
