// Euphoria - Copyright (c) Gustav

/** @file
Camera related code.
 */

#ifndef EUPHORIA_UI_IMAGE_H_
#define EUPHORIA_UI_IMAGE_H_

#include "euphoria/ui-widget.h"

namespace euphoria {
namespace ui {
class Image : public Widget {
 public:
  void Draw() override;
};
}  // namespace ui
}  // namespace euphoria

#endif  // EUPHORIA_UI_IMAGE_H_
