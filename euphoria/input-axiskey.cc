// Euphoria - Copyright (c) Gustav

#include "euphoria/input-axiskey.h"
#include <cassert>
#include "euphoria/input-action.h"

namespace input {

AxisKey::AxisKey(Key::Type minkey, Key::Type maxkey, AxisBase base)
    : minkey_(minkey),
      maxkey_(maxkey),
      minisdown_(false),
      maxisdown_(false),
      base_(base) {}

void AxisKey::OnKey(Key::Type key, bool state) {
  assert(this);
  if (key == minkey_) {
    set_minisdown(state);
  } else {
    assert(key == maxkey_ &&
           "this function only supports arguments of min and max");
    set_maxisdown(state);
  }
}

void AxisKey::set_minisdown(bool minisdown) {
  assert(this);
  minisdown_ = minisdown;
  UpdateState();
}

void AxisKey::set_maxisdown(bool maxisdown) {
  assert(this);
  maxisdown_ = maxisdown;
  UpdateState();
}

void AxisKey::UpdateState() {
  float state = 0.0f;
  if (minisdown_) {
    state -= 1.0f;
  }
  if (maxisdown_) {
    state += 1.0f;
  }
  base_.action()->set_state(base_.Process(state));
}

}  // namespace input
