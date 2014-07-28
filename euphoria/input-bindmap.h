// Euphoria - Copyright (c) Gustav

/** @file
Classes for input handling.
 */

#ifndef EUPHORIA_INPUT_BINDMAP_H_
#define EUPHORIA_INPUT_BINDMAP_H_

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "euphoria/input-actionmap.h"

#include "euphoria/input-bind.h"

#include "euphoria/input-activeaxis.h"
#include "euphoria/input-activerange.h"

class Table;

namespace input {

class InputAction;
class ActiveList;

/** A container for all the actives in the game.
 */
class BindMap {
 public:
  BindMap(const InputActionMap& actions, ActiveList* actives);
  std::shared_ptr<Bind> GetBindByName(const std::string& name);

 private:
  void AddRange(std::shared_ptr<InputAction> action, ActiveList* actives);
  void AddAxis(std::shared_ptr<InputAction> action, ActiveList* actives);

  std::map<std::string, std::shared_ptr<Bind>> binds_;
};

}  // namespace input

#endif  // EUPHORIA_INPUT_BINDMAP_H_
