// Euphoria - Copyright (c) Gustav

/** @file
Camera related code.
 */

#ifndef EUPHORIA_UI_IMAGE_H_
#define EUPHORIA_UI_IMAGE_H_

#include "euphoria/ui-widget.h"
#include "euphoria/ui-enums.h"

namespace euphoria {
class Texture;
class DynamicQuad;
class ShaderCache;
class Settings;

namespace ui {
class Image : public Widget {
 public:
  Image(SizeRule sizerule, Alignment alignment, const Settings& settings,
        ShaderCache* cache, std::shared_ptr<Texture> texture);

  void Draw(const Camera& camera) override;

 private:
  SizeRule sizerule_;
  Alignment alignment_;
  std::shared_ptr<DynamicQuad> quad_;
  // dynamic mesh
};
}  // namespace ui
}  // namespace euphoria

#endif  // EUPHORIA_UI_IMAGE_H_
