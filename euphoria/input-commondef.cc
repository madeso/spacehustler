// Euphoria - Copyright (c) Gustav

#include "euphoria/input-commondef.h"
#include <cassert>
#include <stdexcept>
#include <string>

#include "euphoria/str.h"
#include "json/json.h"

#include "euphoria/input-actionmap.h"
#include "euphoria/input-bindmap.h"
#include "euphoria/input-action.h"
#include "euphoria/stringutils.h"

namespace euphoria {

namespace input {

void TransformAndSetBindValue(const BindData& data, float value) {
  float transformed = value * data.scale;
  if (data.invert) {
    transformed *= -1;
  }
  data.bind->set_value(transformed);
}

CommonDef GetCommonDef(Json::Value& d, const InputActionMap& map) {
  CommonDef common;

  common.bindname = d.get("action", "").asString();
  // verify action
  common.actionname = RemoveFromEnd(
      RemoveFromEnd(RemoveFromEnd(common.bindname, "+"), "-"), "-axis");
  const auto action = map.Get(common.actionname);
  if (common.actionname != common.bindname) {
    const bool needsAxis = false == EndsWith(common.bindname, "-axis");
    // bindname is different from actionname, must be a axis
    bool isAxis = action->range() == Range::INFINITE ||
                  action->range() == Range::WITHIN_NEGATIVE_ONE_POSITIVE_ONE;
    if (isAxis == !needsAxis) {
      const std::string error =
          Str() << "bind " << common.bindname << " looks like a "
                << (needsAxis ? "axis" : "range") << " but isn't for action "
                << common.actionname;
      throw error;
    }
  }
  common.type = d.get("type", "").asString();

  return common;
}

}  // namespace input
}  // namespace euphoria
