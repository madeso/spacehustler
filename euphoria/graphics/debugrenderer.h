// Euphoria - Copyright (c) Gustav

/** @file
Debug rendering.
 */

#ifndef EUPHORIA_GRAPHICS_DEBUGRENDERER_H_
#define EUPHORIA_GRAPHICS_DEBUGRENDERER_H_

#include <vector>

#include "euphoria/color.h"
#include "euphoria/math.h"
#include "euphoria/graphics/openglraii.h"
#include "euphoria/graphics/shader.h"
#include "euphoria/graphics/shadercache.h"
#include "euphoria/graphics/camera.h"

namespace euphoria {

class Settings;

/** The debug renderer.
 */
class DebugRenderer {
 public:
  /** Constructor.
  @param shadercache the shader cache
  @param settings the settings to use
   */
  DebugRenderer(ShaderCache* shadercache, const Settings& settings);

  /** Add a line.
  @param f the from point
  @param t the to point
  @param c the color
   */
  void Line(const Vec3& f, const Vec3& t, const Color& c);

  /** Add a line.
  @param f the from point
  @param t the to point
  @param fc the from color
  @param tc the to color
   */
  void Line(const Vec3& f, const Vec3& t, const Color& fc, const Color& tc);

  /** Render the debug lines.
   */
  void Render(const Camera& camera);

  /** Updates the internal data in the debug renderer.
   */
  void Update();

 private:
  unsigned int render_lines_;
  unsigned int linecount_;
  std::vector<GLfloat> pending_;
  std::vector<GLfloat> points_;
  std::shared_ptr<internal::Vao> vao_;
  std::shared_ptr<internal::ArrayBuffer> vbo_;
  std::shared_ptr<Program> prog_;
};

/** Add a debug view for a matrix.
@param debug the debug renderer
    @param mat the matrix
     */
void Debug(DebugRenderer* debug, const Mat44& mat);

}  // namespace euphoria

#endif  // EUPHORIA_GRAPHICS_DEBUGRENDERER_H_
