// Euphoria - Copyright (c) Gustav

/** @file
Classes for input handling.
 */

#ifndef EUPHORIA_INPUT_ACTIVEMAP_H_
#define EUPHORIA_INPUT_ACTIVEMAP_H_

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "euphoria/input-actionmap.h"

#include "euphoria/input-rangebind.h"
#include "euphoria/input-axisbind.h"

#include "euphoria/input-activeaxis.h"
#include "euphoria/input-activerange.h"
#include "euphoria/input-activerangetoaxis.h"

class Table;

namespace input {

  class InputAction;

/** A container for all the actives in the game.
 */
class ActiveMap {
 public:
   ActiveMap(const InputActionMap& actions);
  std::shared_ptr<AxisBind> range(const std::string& name);
  std::shared_ptr<RangeBind> axis(const std::string& name);
 private:
   void addRange(std::shared_ptr<InputAction> action);
   void addAxis(std::shared_ptr<InputAction> action);

   std::map<std::string, std::shared_ptr<RangeBind>> rangeBinds_;
   std::map<std::string, std::shared_ptr<AxisBind>> axisBinds_;

   std::vector<std::shared_ptr<ActiveRange>> rangeActives_;
   std::vector<std::shared_ptr<ActiveAxis>> axisActives_;
   std::vector<std::shared_ptr<ActiveRangeToAxis>> rangeToAxisActive_;
};

}  // namespace input

#endif  // EUPHORIA_INPUT_ACTIVEMAP_H_
