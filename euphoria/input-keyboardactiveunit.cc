// Euphoria - Copyright (c) Gustav

#include "euphoria/input-keyboardactiveunit.h"
#include <cassert>

#include "euphoria/input-director.h"
#include "euphoria/input-axiskey.h"
#include "euphoria/input-action.h"

namespace input {

  KeyboardActiveUnit::KeyboardActiveUnit(const std::vector<Bind<Key::Type>>& binds,
                    std::vector<AxisKeyBind<Key::Type>> axiskeys,
                     InputDirector* director)
      : director_(director) {
    assert(this);
    assert(director_);

    for (auto b : binds) {
      Add(b.action());
      actions_.insert(std::make_pair(b.type(), b.action()));
    }

    for (auto ak : axiskeys) {
      Add(ak.action());
      std::shared_ptr<AxisKey> axiskey(new AxisKey(ak.min(), ak.max(),
        ak));

      axiskeys_.insert(std::make_pair(ak.min(), axiskey));
      axiskeys_.insert(std::make_pair(ak.max(), axiskey));
    }

    director_->Add(this);
  }

  void KeyboardActiveUnit::OnKey(const Key::Type& key, bool state) {
    assert(this);
    auto actionsit = actions_.find(key);
    if (actionsit != actions_.end()) {
      actionsit->second->set_state(state ? 1.0f : 0.0f);
    }

    auto axiskeysit = axiskeys_.find(key);
    if (axiskeysit != axiskeys_.end()) {
      axiskeysit->second->OnKey(key, state);
    }
  }

  KeyboardActiveUnit::~KeyboardActiveUnit() { director_->Remove(this); }

  void KeyboardActiveUnit::Rumble() {}

}  // namespace input
