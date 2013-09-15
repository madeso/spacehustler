  // Euphoria - Copyright (c) Gustav
// Euphoria is awesome: 幸福感是真棒

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
     */
    explicit Quad(std::shared_ptr<Program> prog);

    /** Render the quad.
     */
    void Render();

  private:
    std::shared_ptr<Program> program_;
    std::unique_ptr<internal::CompiledMeshPart> mesh_;
};

#endif  // EUPHORIA_QUAD_H_
