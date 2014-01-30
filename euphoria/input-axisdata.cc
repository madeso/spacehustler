// Euphoria - Copyright (c) Gustav

#include "euphoria/input-axisdata.h"
#include <cassert>
#include "json/json.h"
#include "euphoria/scalar.h"

namespace input {
   AxisData::AxisData(std::shared_ptr<InputAction> action, const Json::Value& data)
    : AxisBase(action, data), invert_(false) {
      assert(this);
      invert_ = data.get("invert", false).asBool();
      deadzone_ = data.get("deadzone", 0.0f).asFloat();
  }

  float AxisData::Process(float v) const {
    if (Abs(v) < deadzone_) {
      return 0.0f;
    }

    /// @todo make the input start at "0" at the deadzone?
    float r = AxisBase::Process(v);
    if (invert_) {
      r = r * -1.0f;
    }
    return r;
  }

}  // namespace input
