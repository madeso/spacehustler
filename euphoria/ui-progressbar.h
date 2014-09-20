// Euphoria - Copyright (c) Gustav

/** @file
Camera related code.
 */

#ifndef EUPHORIA_UI_PROGRESSBAR_H_
#define EUPHORIA_UI_PROGRESSBAR_H_

#include <memory>

#include "euphoria/ui-widget.h"

namespace euphoria {
class ShaderCache;
class TextureCache;
class Settings;
class NinepatchInstance;
class Ninepatch;
namespace ui {

class ProgressBar : public Widget {
 public:
  ProgressBar(const Ninepatch& ninepatch, TextureCache* texturecache,
              ShaderCache* shadercache, const Settings& settings);

  ~ProgressBar();
  float value() const;
  void set_value(float value);

  void Draw(const Camera& camera) override;

 private:
  std::shared_ptr<NinepatchInstance> ninepatch_;
  float value_;
};
}  // namespace ui
}  // namespace euphoria

#endif  // EUPHORIA_UI_PROGRESSBAR_H_
