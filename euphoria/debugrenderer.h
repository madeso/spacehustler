// Euphoria - Copyright (c) Gustav

/** @file
Debug rendering.
 */

#ifndef EUPHORIA_DEBUGRENDERER_H_
#define EUPHORIA_DEBUGRENDERER_H_

#include <vector>

#include "euphoria/colors.h"  // include colors so that users can use all the colors
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
    void Line(const vec3& f, const vec3& t, const Color& c);

    /** Add a line.
    @param f the from point
    @param t the to point
    @param fc the from color
    @param tc the to color
     */
    void Line(const vec3& f, const vec3& t, const Color& fc, const Color& tc);


    /** Render the debug lines.
     */
    void Render(const Camera& camera);

  protected:
    /** Updates the internal data in the debug renderer.
     */
    void Update();

  private:
    unsigned int linecount_;
    std::vector<GLfloat> pending_;
    std::vector<GLfloat> points_;
    std::shared_ptr<internal::Vao> vao_;
    std::shared_ptr<internal::ArrayBuffer> vbo_;
    std::shared_ptr<Program> prog_;
};

#endif  // EUPHORIA_DEBUGRENDERER_H_
