// Euphoria - Copyright (c) Gustav

#ifndef EUPHORIA_WORLD_H_
#define EUPHORIA_WORLD_H_

#include <boost/shared_ptr.hpp>
#include <vector>

#include "euphoria/mesh.h"
#include "euphoria/math.h"
#include "euphoria/camera.h"

class Instance {
  public:
    Instance(boost::shared_ptr<CompiledMesh> mesh, const mat44& transform);
    void render(const Camera& camera);

    boost::shared_ptr<CompiledMesh> mesh;
    mat44 transform;
};

class World {
  public:
    World();

    void add(boost::shared_ptr<Instance> instance);
    void render(const Camera& camera);

  private:
    std::vector<boost::shared_ptr<Instance>> instances;
};

#endif  // EUPHORIA_WORLD_H_
