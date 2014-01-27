// Euphoria - Copyright (c) Gustav

#include "euphoria/debugrenderer.h"
#include <cassert>
#include <vector>

DebugRenderer::DebugRenderer(ShaderCache* shadercache, const Settings& settings)
  : render_lines_(0), linecount_(0), prog_(shadercache->
      GetOrCreate("debuglines.js", settings)) {
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

void DebugRenderer::Line(const vec3& f, const vec3& t, const Color& c) {
  Line(f, t, c, c);
}

void DebugRenderer::Line(const vec3& f, const vec3& t, const Color& fc,
                         const Color& tc) {
  Add(&pending_, f);
  Add(&pending_, fc);
  Add(&pending_, t);
  Add(&pending_, tc);
  ++linecount_;
}

void DebugRenderer::Update() {
  if (linecount_ != 0) {
    points_ = pending_;
    render_lines_ = linecount_;

    vao_.reset(new internal::Vao());
    vbo_.reset(new internal::ArrayBuffer());

    vao_->Bind();
    vbo_->Bind();

    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * points_.size(), &points_[0],
                 GL_STATIC_DRAW);

    const GLsizei stride = 6 * sizeof(GLfloat);
    const GLvoid* coloroffset = reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat));
    const auto vert = prog_->LookupAttribute("vert");
    const auto color = prog_->LookupAttribute("color");
    glEnableVertexAttribArray(vert);
    glVertexAttribPointer(vert, 3, GL_FLOAT, GL_FALSE, stride, NULL);
    glEnableVertexAttribArray(color);
    glVertexAttribPointer(color, 3, GL_FLOAT, GL_TRUE, stride, coloroffset);

    vbo_->Unbind();
    vao_->Unbind();

    pending_.clear();
    linecount_ = 0;
  }
}

void DebugRenderer::Render(const Camera& camera) {
  if (render_lines_ != 0) {
    prog_->Bind();

    prog_->SetUniform("camera", camera.view());
    prog_->SetUniform("projection", camera.projection());

    vao_->Bind();
    vbo_->Bind();
    glDrawArrays(GL_LINES, 0, render_lines_ * 2);
    vbo_->Unbind();
    vao_->Unbind();
    prog_->Unbind();
  }
}

void Debug(DebugRenderer* debug, const mat44& mat) {
  const float orthoLen = 20;
  vec3 start = matrix_get_translation(mat);
  debug->Line(start, start + matrix_get_x_basis_vector(mat)*orthoLen,
              Color(0.7f, 0, 0));
  debug->Line(start, start + matrix_get_y_basis_vector(mat)*orthoLen,
              Color(0, 0.7f, 0));
  debug->Line(start, start + matrix_get_z_basis_vector(mat)*orthoLen,
              Color(0, 0, 0.7f));
}
