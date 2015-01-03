// Euphoria - Copyright (c) Gustav

/** @file
Camera related code.
 */

#ifndef EUPHORIA_TWEAKRENDERER_H_
#define EUPHORIA_TWEAKRENDERER_H_

#include <memory>

namespace euphoria {

class Fbo;
class Camera;
class Program;
class ShaderCache;
class Settings;

namespace internal {
class CompiledMeshPart;
}  // namespace internal

/** Helper class for rendering tweaks to the world for ovr or to the window.
 */
class TweakRenderer {
 public:
  TweakRenderer(ShaderCache* shadercache, const Settings& settings,
                bool use_ovr, int window_width, int window_height);
  ~TweakRenderer();

  void PreRender();
  void Render(const Camera& cam);

 private:
  std::unique_ptr<internal::CompiledMeshPart> quad_;
  std::unique_ptr<Fbo> fbo_;
  std::shared_ptr<Program> program_;
};

}  // namespace euphoria

#endif  // EUPHORIA_TWEAKRENDERER_H_
