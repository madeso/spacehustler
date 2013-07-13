// Euphoria - Copyright (c) Gustav

/** @file
Debug rendering.
 */

#ifndef EUPHORIA_DEBUGRENDERER_H_
#define EUPHORIA_DEBUGRENDERER_H_

#include <vector>

#include "euphoria/color.h"
#include "euphoria/math.h"

/** The debug renderer.
 */
class DebugRenderer {
  public:
    /** Constructor.
     */
    DebugRenderer();

    /** Add a line.
    @param f the from point
    @param t the to point
    @param c the color
     */
    void addLine(const vec3& f, const vec3& t, const Color& c);

    /** Render the debug lines.
     */
    void render();

  private:
    std::vector<float> pending;
    std::vector<float> points;
};

#endif  // EUPHORIA_DEBUGRENDERER_H_
