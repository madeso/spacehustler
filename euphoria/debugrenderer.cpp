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
}  // namespace

void DebugRenderer::line(const vec3& f, const vec3& t, const Color& c) {
  Add(&pending, f);
  Add(&pending, t);
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
  auto vert = prog->attrib("vert");
  glEnableVertexAttribArray(vert);
  glVertexAttribPointer(vert, 3, GL_FLOAT, GL_FALSE, 0, NULL);

  vbo->unbind();
  vao->unbind();

  pending.clear();
}

void DebugRenderer::render(const Camera& camera) {
  if (linecount != 0) {
    update();

    prog->bind();

    prog->setUniform("camera", camera.view);
    prog->setUniform("projection", camera.getProjection());

    vao->bind();
    vbo->bind();
    glDrawArrays(GL_LINES, 0, linecount * 2);
    vbo->unbind();
    vao->unbind();
    prog->unbind();

    linecount = 0;
  }
}
