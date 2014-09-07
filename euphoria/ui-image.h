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
class TextureCache;

namespace ui {
class Image : public Widget {
 public:
  explicit Image(std::shared_ptr<Texture> texture);

  void Draw(const Camera& camera) override;

  SizeRule sizerule() const;
  void set_sizerule(SizeRule sizerule);

 private:
  SizeRule sizerule_;
  std::shared_ptr<Texture> texture_;

  // dynamic mesh
};
}  // namespace ui
}  // namespace euphoria

#endif  // EUPHORIA_UI_IMAGE_H_
