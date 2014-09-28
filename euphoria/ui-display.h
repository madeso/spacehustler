// Euphoria - Copyright (c) Gustav

/** @file
Camera related code.
 */

#include <vector>
#include <memory>
#include <string>

#ifndef EUPHORIA_UI_DISPLAY_H_
#define EUPHORIA_UI_DISPLAY_H_

namespace euphoria {
class TextureCache;
class ShaderCache;
class Settings;

namespace ui {

class Container;

class Display {
 public:
  Display(const std::string& file, int width, int height, TextureCache* tcache,
          ShaderCache* scache, const Settings& settings);
  void SetSize(int width, int height);
  void Render();

 private:
  int width_;
  int height_;
  std::shared_ptr<Container> container_;
};
}  // namespace ui
}  // namespace euphoria

#endif  // EUPHORIA_UI_DISPLAY_H_
