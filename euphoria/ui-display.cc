// Euphoria - Copyright (c) Gustav

#include "euphoria/ui-display.h"

#include <cassert>

#include "euphoria/opengl.h"
#include "euphoria/ogldebug.h"
#include "euphoria/camera.h"

#include "euphoria/ui-container.h"

namespace euphoria {
namespace ui {

namespace detail {
std::shared_ptr<Container> LoadContainer(const std::string& file,
                                         TextureCache* tcache,
                                         ShaderCache* scache,
                                         const Settings& settings);
}  // namespace detail

Display::Display(const std::string& file, int width, int height,
                 TextureCache* tcache, ShaderCache* scache,
                 const Settings& settings)
    : width_(width),
      height_(height),
      container_(detail::LoadContainer(file, tcache, scache, settings)) {
  assert(this);

  /// @todo create container

  SetSize(width, height);
}

void Display::SetSize(int width, int height) {
  assert(this);
  assert(container_);

  assert(width > 0);
  assert(height > 0);

  width_ = width;
  height_ = height;

  container_->set_position(Vec2(0, 0));
  container_->set_size(
      Vec2(static_cast<float>(width), static_cast<float>(height)));
  container_->Layout();
}

void Display::Render() {
  assert(this);
  assert(container_);

  /// @todo introduce curved quad rendering, like
  /// https://www.assetstore.unity3d.com/en/#!/content/19956
  Camera camera = CreateCameraOrtho(width_, height_, NearFar(0.1f, 10.0f));
  glDisable(GL_DEPTH_TEST);
  container_->Draw(camera);
  glEnable(GL_DEPTH_TEST);
}

}  // namespace ui
}  // namespace euphoria
