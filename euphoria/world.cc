// Euphoria - Copyright (c) Gustav

#include "euphoria/world.h"
#include <cassert>
#include <string>
#include <fstream> // NOLINT for loading data
#include <stdexcept>
#include "euphoria/str.h"
#include "json/json.h"

Instance::Instance(std::shared_ptr<CompiledMesh> m, const mat44& t)
  : mesh(m)
  , transform(t) {  // NOLINT
}

void Instance::render(const Camera& camera) {
  assert(this);
  mesh->render(camera, transform);
}

World::World(const std::string& filename, TextureCache* texturecache,
             ShaderCache* shadercache) : debugrenderer(shadercache) {
  std::ifstream in(filename.c_str());
  if (!in.good()) {
    throw std::logic_error(Str()
                           << "Unable to load definitions from " << filename);
  }
  Json::Value root;
  Json::Reader reader;
  if (false == reader.parse(in, root)) {
    throw std::logic_error(Str() << "Unable to parse " << filename << ": "
                           << reader.getFormattedErrorMessages());
  }

  const std::string collisionmeshfilename = root
      .get("collisionmesh", "").asString();
  if (false == collisionmeshfilename.empty()) {
    collisionmesh = LoadMesh(collisionmeshfilename);
  }
  Json::Value meshes = root["meshes"];
  for (Json::ArrayIndex i = 0; i < meshes.size(); ++i) {
    const std::string meshfile = meshes[i].asString();

    std::shared_ptr<CompiledMesh> mworld(new CompiledMesh(LoadMesh(meshfile),
                                         texturecache,
                                         shadercache));
    mat44 worldmat = cmat44(cvec3zero());

    std::shared_ptr<Instance> wi(new Instance(mworld, worldmat));
    add(wi);
  }
}

void World::add(std::shared_ptr<Instance> instance) {
  assert(this);
  instances.push_back(instance);
}

void World::render(const Camera& camera) {
  assert(this);

  /// @todo investigate http://www.opengl.org/registry/specs/ARB/draw_instanced.txt
  for (auto i : instances) {
    i->render(camera);
  }

  debugrenderer.Render(camera);
}

DebugRenderer& World::debug() {
  return debugrenderer;
}

const Mesh& World::getCollisionMesh() const {
  assert(this);
  return collisionmesh;
}
