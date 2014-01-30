// Euphoria - Copyright (c) Gustav

/** @file
Classes for input handling.
 */

#ifndef EUPHORIA_INPUT_ACTIONMAP_H_
#define EUPHORIA_INPUT_ACTIONMAP_H_

#include <map>
#include <string>
#include <memory>

class Table;

namespace input {

  class InputAction;

/** A map of all the available input actions.
 */
class InputActionMap {
 public:
  /** Constructor.
   */
  InputActionMap();

  /** Add a action.
  @param name the name of the action
  @param action the action itself
   */
  void Add(const std::string& name, std::shared_ptr<InputAction> action);

  /** Get a certain action.
  @param name the name of the action
  @returns the input action
   */
  std::shared_ptr<InputAction> Get(const std::string& name) const;

 private:
  std::map<std::string, std::shared_ptr<InputAction>> actions_;
};

/** Load actions from file.
@param map the action map to fill
@param filename the filename to load from
 */
void Load(InputActionMap* map, const std::string& filename);

}  // namespace input

#endif  // EUPHORIA_INPUT_ACTIONMAP_H_
