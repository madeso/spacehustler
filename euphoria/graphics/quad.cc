// Euphoria - Copyright (c) Gustav

#include "euphoria/graphics/quad.h"

#include <cassert>

namespace euphoria {

std::unique_ptr<internal::CompiledMeshPart> MakeQuad(
    std::shared_ptr<Program> program_, float width, float height, float z,
    float uscale, float vscale) {
  const float xsize = width / 2.0f;
  const float ysize = height / 2.0f;
  internal::MeshPart part;
  part.AddPoint(-xsize, ysize, z, 0.0f, vscale);
  part.AddPoint(xsize, ysize, z, uscale, vscale);
  part.AddPoint(xsize, -ysize, z, uscale, 0.0f);
  part.AddPoint(-xsize, -ysize, z, 0.0f, 0.0f);

  part.AddFace(2, 1, 0);
  part.AddFace(3, 2, 0);
  std::unique_ptr<internal::CompiledMeshPart> mesh(
      new ::euphoria::internal::CompiledMeshPart(part, program_, 0));
  return mesh;
}

}  // namespace euphoria
