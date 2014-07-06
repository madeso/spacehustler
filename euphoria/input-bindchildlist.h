// Euphoria - Copyright (c) Gustav

/** @file
Classes for input handling.
 */

#ifndef EUPHORIA_INPUT_BINDCHILDLIST_H_
#define EUPHORIA_INPUT_BINDCHILDLIST_H_

#include <vector>
#include <memory>

#include "euphoria/input-activeaxis.h"
#include "euphoria/input-activerange.h"
#include "euphoria/input-activerangetoaxis.h"

namespace input {

/** A container for all the actives in the game.
 */
class BindChildList {
 public:
  void add(std::shared_ptr<ActiveRange> range);
  void add(std::shared_ptr<ActiveAxis> axis);
  void add(std::shared_ptr<ActiveRangeToAxis> axis);

  void update(float dt);

 private:
  std::vector<std::shared_ptr<ActiveRange>> rangeBinds_;
  std::vector<std::shared_ptr<ActiveAxis>> axisBinds_;
  std::vector<std::shared_ptr<ActiveRangeToAxis>> rangeToAxisBinds_;
};

}  // namespace input

#endif  // EUPHORIA_INPUT_BINDCHILDLIST_H_
