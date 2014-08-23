// Euphoria - Copyright (c) Gustav

#include "euphoria/ui-value.h"

#include <cassert>

#include "euphoria/scalar.h"

namespace euphoria {
namespace ui {

Value::Value() : value_(0.0f), unit_(Unit::PIXEL) { assert(this); }

Value::Value(float value, Unit unit) : value_(value), unit_(unit) {
  assert(this);
  if (unit == Unit::RELATIVE) {
    assert(IsWithinInclusive(0.0f, value, 1.0f));
  }
}

float Value::value(float parent_value) const {
  assert(this);
  switch (unit_) {
    case Unit::PIXEL:
      return value_;
    case Unit::RELATIVE:
      return value_ * parent_value;
    case Unit::AVAILABLE:
      assert(false && "unable to determine value, should use other function");
      return 0.0f;
    default:
      assert(false && "unknown unit");
      return 0.0f;
  }
}

bool Value::IsRestPercentage() const {
  assert(this);
  return unit_ == Unit::AVAILABLE;
}

float Value::GetRestPercentage() const {
  assert(this);
  assert(unit_ == Unit::AVAILABLE);
  return value_;
}

}  // namespace ui
}  // namespace euphoria
