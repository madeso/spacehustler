// Euphoria - Copyright (c) Gustav

/** @file
Camera related code.
 */

#ifndef EUPHORIA_UI_PADDING_H_
#define EUPHORIA_UI_PADDING_H_

#include "euphoria/math.h"
#include "euphoria/ui-value.h"

namespace euphoria {
namespace ui {

class Padding {
 public:
  Padding(Value left, Value right, Value up, Value down);
  void ModifyWidgetLayout(Vec2* pos, Vec2* size) const;

 private:
  Value left_;
  Value right_;
  Value up_;
  Value down_;
};

}  // namespace ui
}  // namespace euphoria

#endif  // EUPHORIA_UI_PADDING_H_
