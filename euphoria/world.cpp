// Euphoria - Copyright (c) Gustav

#include "euphoria/world.h"
#include <cassert>

Instance::Instance(boost::shared_ptr<CompiledMesh> m, const mat44& t)
  : mesh(m)
  , transform(t) {  // NOLINT
}

void Instance::render(const Camera& camera) {
  assert(this);
  mesh->render(camera, transform);
}

World::World() {
}

void World::add(boost::shared_ptr<Instance> instance) {
  assert(this);
  instances.push_back(instance);
}

void World::render(const Camera& camera) {
  assert(this);

  /// @todo investigate http://www.opengl.org/registry/specs/ARB/draw_instanced.txt
for (auto i : instances) {
    i->render(camera);
  }
}
