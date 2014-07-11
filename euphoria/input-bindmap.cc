// Euphoria - Copyright (c) Gustav

#include "euphoria/input-bindmap.h"
#include <cassert>

#include "euphoria/input-action.h"
#include "euphoria/input-range.h"
#include "euphoria/input-activelist.h"

namespace input {

  BindMap::BindMap(const InputActionMap& actions, ActiveList* actives) {
  assert(this);
  std::shared_ptr<InputAction> action;

  switch (action->range()) {
    case Range::Infinite:
    case Range::WithinNegative1Positive1:
      addAxis(action, actives);
      break;
    case Range::Within01:
      addRange(action, actives);
      break;
    default:
      throw "Invalid range type";
  }
}

  void BindMap::addRange(std::shared_ptr<InputAction> action, ActiveList* actives) {
  assert(this);
  std::string actionname;

  /// @todo verify that it is indeed a range

  std::shared_ptr<RangeBind> bind(new RangeBind(action.get()));
  std::shared_ptr<ActiveRange> active(
      new ActiveRange(action.get(), bind.get()));
  actives->add(active);
  rangeBinds_.insert(std::make_pair(actionname, bind));
}

  void BindMap::addAxis(std::shared_ptr<InputAction> action, ActiveList* actives) {
  assert(this);
  std::string actionname;

  /// @todo verify that it is indeed a axis

  std::shared_ptr<AxisBind> bind(new AxisBind(action.get()));
  std::shared_ptr<ActiveAxis> active(new ActiveAxis(action.get(), bind.get()));
  actives->add(active);
  axisBinds_.insert(std::make_pair(actionname, bind));

  std::shared_ptr<RangeBind> bindNeg(new RangeBind(action.get()));
  std::shared_ptr<RangeBind> bindPos(new RangeBind(action.get()));
  std::shared_ptr<ActiveRangeToAxis> activeAxis(
      new ActiveRangeToAxis(action.get(), bindPos.get(), bindNeg.get()));
  actives->add(activeAxis);
  rangeBinds_.insert(std::make_pair(actionname + "-", bindNeg));
  rangeBinds_.insert(std::make_pair(actionname + "+", bindPos));
}

}  // namespace input
