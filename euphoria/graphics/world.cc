// Euphoria - Copyright (c) Gustav
#include "euphoria/world.h"
#include <cassert>
#include <string>
#include <fstream>  // NOLINT for loading data
#include <stdexcept>
#include "euphoria/str.h"
#include "json/json.h"

namespace euphoria {

Instance::Instance(std::shared_ptr<CompiledMesh> mesh, const Mat44& transform)
    : mesh_(mesh), transform_(transform) {  // NOLINT
}

void Instance::Render(const Camera& camera) {
  assert(this);
  mesh_->Render(camera, transform_);
}

const Mat44& Instance::transform() const {
  assert(this);
  return transform_;
}
void Instance::set_transform(const Mat44& transform) {
  assert(this);
  transform_ = transform;
}

World::World(const std::string& filename, TextureCache* texturecache,
             ShaderCache* shadercache, const Settings& settings)
    : debug_transforms_(true), debug_renderer_(shadercache, settings) {
  std::ifstream in(filename.c_str());
  if (!in.good()) {
    throw std::logic_error(Str() << "Unable to load definitions from "
                                 << filename);
  }
  Json::Value root;
  Json::Reader reader;
  if (false == reader.parse(in, root)) {
    throw std::logic_error(Str() << "Unable to parse " << filename << ": "
                                 << reader.getFormattedErrorMessages());
  }

  const std::string collisionmeshfilename =
      root.get("collisionmesh", "").asString();
  if (false == collisionmeshfilename.empty()) {
    collisionmesh_ = LoadMesh(collisionmeshfilename);
  }
  Json::Value meshes = root["meshes"];
  for (Json::ArrayIndex i = 0; i < meshes.size(); ++i) {
    const std::string meshfile = meshes[i].asString();

    std::shared_ptr<CompiledMesh> mworld(new CompiledMesh(
        LoadMesh(meshfile), texturecache, shadercache, settings));
    Mat44 worldmat = CreateMat44(CreateZeroedVec3());

    std::shared_ptr<Instance> wi(new Instance(mworld, worldmat));
    Add(wi);
  }
}

void World::Add(std::shared_ptr<Instance> instance) {
  assert(this);
  instances_.push_back(instance);
}

void World::Render(const Camera& camera) {
  assert(this);

  /// @todo investigate
  /// http://www.opengl.org/registry/specs/ARB/draw_instanced.txt
  for (auto i : instances_) {
    i->Render(camera);
    if (debug_transforms_) {
      Debug(&debug_renderer_, i->transform());
    }
  }

  debug_renderer_.Render(camera);
}

DebugRenderer& World::debug_renderer() { return debug_renderer_; }

const Mesh& World::collisionmesh() const {
  assert(this);
  return collisionmesh_;
}

}  // namespace euphoria
