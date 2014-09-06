// Euphoria - Copyright (c) Gustav

#include "euphoria/ui-display.h"

#include <cassert>

namespace euphoria {
namespace ui {

Display::Display(const std::string& file, float width, float height,
                 TextureCache* tcache, ShaderCache* scache,
                 const Settings& settings) {
  LoadTable(&table_, file, tcache, scache, settings);
  table_.LayoutCells(width, height);
}

void Display::Render() { table_.Render(); }

}  // namespace ui
}  // namespace euphoria
