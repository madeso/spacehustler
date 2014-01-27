// Euphoria - Copyright (c) Gustav

/** @file
Quad related code.
 */

#ifndef EUPHORIA_QUAD_H_
#define EUPHORIA_QUAD_H_

#include <memory>

#include "euphoria/mesh.h"

/** Utility to render a quad.
 */
class Quad {
  public:
    /** Constructor.
    @param prog the program to use
    @param uscale the U scale
    @param vscale the V scale
     */
    Quad(std::shared_ptr<Program> prog, float uscale, float vscale);

    /** Render the quad.
     */
    void Render();

  private:
    std::shared_ptr<Program> program_;
    std::unique_ptr<internal::CompiledMeshPart> mesh_;
};

#endif  // EUPHORIA_QUAD_H_
