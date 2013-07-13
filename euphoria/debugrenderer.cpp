// Euphoria - Copyright (c) Gustav

#include "euphoria/debugrenderer.h"
#include <cassert>
#include <vector>

DebugRenderer::DebugRenderer() {
}

namespace {
  void Add(std::vector<float>* d, const vec3& p) {
    d->push_back(p[0]);
    d->push_back(p[1]);
    d->push_back(p[2]);
  }
}  // namespace

void DebugRenderer::addLine(const vec3& f, const vec3& t, const Color& c) {
  Add(&pending, f);
  Add(&pending, t);
}

void DebugRenderer::render() {
  points = pending;
  pending.clear();
}
