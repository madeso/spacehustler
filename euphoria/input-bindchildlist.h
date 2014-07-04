// Euphoria - Copyright (c) Gustav

/** @file
Classes for input handling.
 */

#ifndef EUPHORIA_INPUT_BINDCHILDLIST_H_
#define EUPHORIA_INPUT_BINDCHILDLIST_H_

#include <vector>
#include <memory>

#include "euphoria/input-rangebind.h"
#include "euphoria/input-axisbind.h"

namespace input {

/** A axis bound to two keys.
 */
class BindChildList {
 public:
  void add(std::shared_ptr<RangeBind> range);
  void add(std::shared_ptr<AxisBind> axis);
  void update(float dt);

 private:
  std::vector<std::shared_ptr<RangeBind>> rangeBinds_;
  std::vector<std::shared_ptr<AxisBind>> axisBinds_;
};

}  // namespace input

#endif  // EUPHORIA_INPUT_BINDCHILDLIST_H_
