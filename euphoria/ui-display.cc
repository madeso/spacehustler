// Euphoria - Copyright (c) Gustav

#include "euphoria/ui-display.h"

#include <cassert>

#include "euphoria/opengl.h"
#include "euphoria/ogldebug.h"

#include "euphoria/camera.h"

namespace euphoria {
namespace ui {

Display::Display(const std::string& file, int width, int height,
                 TextureCache* tcache, ShaderCache* scache,
                 const Settings& settings)
    : width_(width), height_(height) {
  LoadTable(&table_, file, tcache, scache, settings);
  table_.LayoutCells(width, height);
}

void Display::Render() {
  Camera camera = CreateCameraOrtho(width_, height_, NearFar(0.1f, 10.0f));
  glDisable(GL_DEPTH_TEST);
  table_.Render(camera);
  glEnable(GL_DEPTH_TEST);
}

}  // namespace ui
}  // namespace euphoria
