// Euphoria - Copyright (c) Gustav

/** @file
Camera related code.
 */

#ifndef EUPHORIA_UI_WIDGET_PROGRESSBAR_H_
#define EUPHORIA_UI_WIDGET_PROGRESSBAR_H_

#include <memory>

#include "euphoria/ui-widget.h"

namespace euphoria {
class ShaderCache;
class TextureCache;
class Settings;
class NinepatchInstance;
class Ninepatch;
namespace ui {

enum class ProgressbarLayout { HORIZONTAL, VERTICAL };

class ProgressbarWidget : public Widget {
 public:
  ProgressbarWidget(const Ninepatch& foreground, const Ninepatch& background,
                    TextureCache* texturecache, ShaderCache* shadercache,
                    const Settings& settings);

  ~ProgressbarWidget();

  float value() const;
  void set_value(float value);

  ProgressbarLayout layout() const;
  void set_layout(ProgressbarLayout layout);

  void Layout() override;

  void Draw(const Camera& camera) override;

 private:
  std::shared_ptr<NinepatchInstance> foreground_;
  std::shared_ptr<NinepatchInstance> background_;
  float value_;
  ProgressbarLayout layout_;
};
}  // namespace ui
}  // namespace euphoria

#endif  // EUPHORIA_UI_WIDGET_PROGRESSBAR_H_
