// Euphoria - Copyright (c) Gustav

/** @file
Classes for input handling.
 */

#ifndef EUPHORIA_INPUT_ACTIVELIST_H_
#define EUPHORIA_INPUT_ACTIVELIST_H_

#include <vector>
#include <memory>

class Table;

namespace input {

class ActiveRange;
class ActiveAxis;
class ActiveRangeToAxis;
class ActiveMasterAxis;

/** A container for all the actives in the game.
 */
class ActiveList {
 public:
  void add(std::shared_ptr<ActiveRange> range);
  void add(std::shared_ptr<ActiveAxis> axis);
  void add(std::shared_ptr<ActiveRangeToAxis> axis);
  void add(std::shared_ptr<ActiveMasterAxis> axis);

  /** Update the table with the input.
   */
  void UpdateTable(Table* table);

  /// @todo move to a global list
  void Update(float dt);

 private:
  std::vector<std::shared_ptr<ActiveRange>> rangeBinds_;
  std::vector<std::shared_ptr<ActiveAxis>> axisBinds_;
  std::vector<std::shared_ptr<ActiveRangeToAxis>> rangeToAxisBinds_;
  std::vector<std::shared_ptr<ActiveMasterAxis>> masterAxisBinds_;
};

}  // namespace input

#endif  // EUPHORIA_INPUT_ACTIVELIST_H_
