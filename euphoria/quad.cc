  // Euphoria - Copyright (c) Gustav
// Euphoria is awesome: 幸福感是真棒

#include "euphoria/quad.h"

#include <cassert>

Quad::Quad(std::shared_ptr<Program> prog)
  : program_(prog) {
  internal::MeshPart part;
  const float z = 0.5f;
  part.AddPoint(-1.0f,  1.0f, z, 0.0f, 0.0f);
  part.AddPoint(1.0f,  1.0f, z, 1.0f, 0.0f);
  part.AddPoint(1.0f, -1.0f, z, 1.0f, 1.0f);
  part.AddPoint(-1.0f, -1.0f, z, 0.0f, 1.0f);

  part.AddFace(2, 1, 0);
  part.AddFace(3, 2, 0);
  mesh_.reset(new ::internal::CompiledMeshPart(part, program_, 0));
}

void Quad::Render() {
  mesh_->Render();
}
