// Euphoria - Copyright (c) Gustav

/** @file
Camera related code.
 */

#ifndef EUPHORIA_UI_PROGRESSBAR_H_
#define EUPHORIA_UI_PROGRESSBAR_H_

#include "euphoria/ui-widget.h"

namespace euphoria {
namespace ui {
class ProgressBar : public Widget {
 public:
  float value() const;
  void set_value(float value);

  void Draw() override;
 private:
  float value_;
};
}  // namespace ui
}  // namespace euphoria

#endif  // EUPHORIA_UI_PROGRESSBAR_H_
