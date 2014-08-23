// Euphoria - Copyright (c) Gustav

/** @file
Camera related code.
 */

#include "euphoria/ui-enums.h"

#ifndef EUPHORIA_UI_VALUE_H_
#define EUPHORIA_UI_VALUE_H_

namespace euphoria {
namespace ui {
class Value {
 public:
  Value();
  Value(float value, Unit unit);
  float value(float parent_value) const;

  bool IsRestPercentage() const;
  float GetRestPercentage() const;

 private:
  float value_;
  Unit unit_;
};
}  // namespace ui
}  // namespace euphoria

#endif  // EUPHORIA_UI_VALUE_H_
