// Euphoria - Copyright (c) Gustav

/** @file
Quad related code.
 */

#ifndef EUPHORIA_GRAPHICS_QUAD_H_
#define EUPHORIA_GRAPHICS_QUAD_H_

#include <memory>

#include "euphoria/graphics/mesh.h"

namespace euphoria {

/** Utility to render a quad.
  @param prog the program to use
  @param uscale the U scale
  @param vscale the V scale
   */
std::unique_ptr<internal::CompiledMeshPart> MakeQuad(
    std::shared_ptr<Program> prog, float width, float height, float z,
    float uscale, float vscale);

}  // namespace euphoria

#endif  // EUPHORIA_GRAPHICS_QUAD_H_
