// Euphoria - Copyright (c) Gustav

/** @file
Debug rendering.
 */

#ifndef EUPHORIA_DEBUGRENDERER_H_
#define EUPHORIA_DEBUGRENDERER_H_

#include <vector>

#include "euphoria/color.h"
#include "euphoria/math.h"
#include "euphoria/openglraii.h"
#include "euphoria/shader.h"
#include "euphoria/shadercache.h"
#include "euphoria/camera.h"

/** The debug renderer.
 */
class DebugRenderer {
  public:
    /** Constructor.
     */
    explicit DebugRenderer(ShaderCache* shadercache);

    /** Add a line.
    @param f the from point
    @param t the to point
    @param c the color
     */
    void line(const vec3& f, const vec3& t, const Color& c);

    /** Render the debug lines.
     */
    void render(const Camera& camera);

  protected:
    /** Updates the internal data in the debug renderer.
     */
    void update();

  private:
    unsigned int linecount;
    std::vector<GLfloat> pending;
    std::vector<GLfloat> points;
    std::shared_ptr<internal::Vao> vao;
    std::shared_ptr<internal::ArrayBuffer> vbo;
    std::shared_ptr<Program> prog;
};

#endif  // EUPHORIA_DEBUGRENDERER_H_
