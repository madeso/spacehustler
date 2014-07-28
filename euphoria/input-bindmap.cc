// Euphoria - Copyright (c) Gustav

#include "euphoria/input-bindmap.h"
#include <cassert>

#include "euphoria/input-action.h"
#include "euphoria/input-range.h"
#include "euphoria/input-activelist.h"
#include "euphoria/input-activemasteraxis.h"
#include "euphoria/input-activemasterrange.h"

#include "euphoria/input-activerangetoaxis.h"
#include "euphoria/input-activeaxistorange.h"

#include "euphoria/str.h"

namespace input {

BindMap::BindMap(const InputActionMap& actions, ActiveList* actives) {
  assert(this);
  const auto as = actions.GetActionList();

  for (auto action : as) {
    auto range = action->range();
    switch (range) {
      case Range::Infinite:
      case Range::WithinNegative1Positive1:
        AddAxis(action, actives);
        break;
      case Range::Within01:
        AddRange(action, actives);
        break;
      default:
        const std::string error = Str() << "Invalid range type " << range;
        throw error;
    }
  }
}

std::shared_ptr<Bind> BindMap::GetBindByName(const std::string& name) {
  auto res = binds_.find(name);
  if (res == binds_.end()) {
    const std::string error = Str() << "unable to find bind " << name;
    throw error;
  }
  return res->second;
}

void BindMap::AddRange(std::shared_ptr<InputAction> action,
                       ActiveList* actives) {
  assert(this);
  std::string actionname = action->name();

  /// @todo verify that it is indeed a range

  std::shared_ptr<Bind> bind(new Bind(action.get(), BindType::Range));
  std::shared_ptr<ActiveRange> active(
      new ActiveRange(action.get(), bind.get()));
  actives->Add(active);
  binds_.insert(std::make_pair(actionname, bind));

  bind.reset(new Bind(action.get(), BindType::Axis));
  std::shared_ptr<ActiveAxisToRange> activeAxis(
      new ActiveAxisToRange(action.get(), bind.get()));
  actives->Add(activeAxis);
  binds_.insert(std::make_pair(actionname + "-axis", bind));

  std::shared_ptr<ActiveMasterRange> masterRange(
      new ActiveMasterRange(action.get(), active.get(), activeAxis.get()));
  actives->Add(masterRange);
}

void BindMap::AddAxis(std::shared_ptr<InputAction> action,
                      ActiveList* actives) {
  assert(this);
  std::string actionname = action->name();

  /// @todo verify that it is indeed a axis

  std::shared_ptr<Bind> bind(new Bind(action.get(), BindType::Axis));
  std::shared_ptr<ActiveAxis> active(new ActiveAxis(action.get(), bind.get()));
  actives->Add(active);
  binds_.insert(std::make_pair(actionname, bind));

  std::shared_ptr<Bind> bindNeg(new Bind(action.get(), BindType::Range));
  std::shared_ptr<Bind> bindPos(new Bind(action.get(), BindType::Range));
  std::shared_ptr<ActiveRangeToAxis> activeAxis(
      new ActiveRangeToAxis(action.get(), bindPos.get(), bindNeg.get()));
  actives->Add(activeAxis);
  binds_.insert(std::make_pair(actionname + "-", bindNeg));
  binds_.insert(std::make_pair(actionname + "+", bindPos));

  std::shared_ptr<ActiveMasterAxis> masterAxis(
      new ActiveMasterAxis(action.get(), active.get(), activeAxis.get()));
  actives->Add(masterAxis);
}

}  // namespace input
