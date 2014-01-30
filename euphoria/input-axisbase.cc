// Euphoria - Copyright (c) Gustav

#include "euphoria/input-axisbase.h"
#include <cassert>
#include "json/json.h"

namespace input {

/** Def common between axis and key-minmax axis during activity.
 */

AxisBase::AxisBase(std::shared_ptr<InputAction> action, const Json::Value& data)
    : action_(action), scale_(1.0f) {
  set_scale(data.get("scale", 1.0f).asFloat());
}

std::shared_ptr<InputAction> AxisBase::action() const {
  assert(this);
  return action_;
}

float AxisBase::scale() const {
  assert(this);
  return scale_;
}

void AxisBase::set_scale(float scale) {
  assert(this);
  scale_ = scale;
}

// smoothing?

float AxisBase::Process(float v) const {
  float r = v * scale();
  return r;
}

}  // namespace input
