// Euphoria - Copyright (c) Gustav

#include "euphoria/input-activemap.h"
#include <cassert>

#include "euphoria/input-action.h"
#include "euphoria/input-range.h"

namespace input {

  ActiveMap::ActiveMap(const InputActionMap& actions) {
    assert(this);
    std::shared_ptr<InputAction> action;

    switch (action->range()) {
    case Range::Infinite:
    case Range::WithinNegative1Positive1:
      addAxis(action);
      break;
    case Range::Within01:
      addRange(action);
      break;
    default:
      throw "Invalid range type";
    }
  }

  void ActiveMap::addRange(std::shared_ptr<InputAction> action) {
    assert(this);
    std::string actionname;

    /// @todo verify that it is indeed a range

    std::shared_ptr<RangeBind> bind(new RangeBind(action.get()));
    std::shared_ptr<ActiveRange> active(new ActiveRange(action.get(), bind.get()));
    rangeActives_.push_back(active);
    rangeBinds_.insert(std::make_pair(actionname, bind));
  }

  void ActiveMap::addAxis(std::shared_ptr<InputAction> action) {
    assert(this);
    std::string actionname;

    /// @todo verify that it is indeed a axis

    std::shared_ptr<AxisBind> bind(new AxisBind(action.get()));
    std::shared_ptr<ActiveAxis> active(new ActiveAxis(action.get(), bind.get()));
    axisActives_.push_back(active);
    axisBinds_.insert(std::make_pair(actionname, bind));

    std::shared_ptr<RangeBind> bindNeg(new RangeBind(action.get()));
    std::shared_ptr<RangeBind> bindPos(new RangeBind(action.get()));
    std::shared_ptr<ActiveRangeToAxis> activeAxis(new ActiveRangeToAxis(action.get(), bindPos.get(), bindNeg.get()));
    rangeToAxisActive_.push_back(activeAxis);
    rangeBinds_.insert(std::make_pair(actionname + "-", bindNeg));
    rangeBinds_.insert(std::make_pair(actionname + "+", bindPos));
  }

  /*
  std::map<std::string, std::shared_ptr<RangeBind>> rangeBinds_;
  std::map<std::string, std::shared_ptr<AxisBind>> axisBinds_;

  std::vector<std::shared_ptr<ActiveRange>> rangeActives_;
  std::vector<std::shared_ptr<ActiveAxis>> axisActives_;
  std::vector<std::shared_ptr<ActiveRangeToAxis>> rangeToAxisActive_;
  */

}  // namespace input
