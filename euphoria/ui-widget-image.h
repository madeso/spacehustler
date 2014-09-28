// Euphoria - Copyright (c) Gustav

/** @file
Camera related code.
 */

#ifndef EUPHORIA_UI_WIDGET_IMAGE_H_
#define EUPHORIA_UI_WIDGET_IMAGE_H_

#include "euphoria/ui-widget.h"
#include "euphoria/ui-enums.h"

namespace euphoria {
class Texture;
class DynamicQuad;
class ShaderCache;
class Settings;

namespace ui {
class ImageWidget : public Widget {
 public:
  ImageWidget(const Settings& settings, ShaderCache* cache,
              std::shared_ptr<Texture> texture);

  void Layout() override;

  void Draw(const Camera& camera) override;

 private:
  std::shared_ptr<DynamicQuad> quad_;
  // dynamic mesh
};
}  // namespace ui
}  // namespace euphoria

#endif  // EUPHORIA_UI_WIDGET_IMAGE_H_
