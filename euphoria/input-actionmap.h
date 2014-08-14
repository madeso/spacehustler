// Euphoria - Copyright (c) Gustav

/** @file
Classes for input handling.
 */

#ifndef EUPHORIA_INPUT_ACTIONMAP_H_
#define EUPHORIA_INPUT_ACTIONMAP_H_

#include <map>
#include <string>
#include <memory>
#include <vector>

namespace euphoria {

class Table;

namespace input {

class InputAction;
class GlobalToggle;

/** A map of all the available input actions.
 */
class InputActionMap {
 public:
  /** Constructor.
   */
  InputActionMap();

  void Update();

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
  std::shared_ptr<GlobalToggle> GetGlobalToggle(const std::string& name) const;

  std::vector<std::shared_ptr<InputAction>> GetActionList() const;

 private:
  std::map<std::string, std::shared_ptr<InputAction>> actions_;
  std::map<std::string, std::shared_ptr<GlobalToggle>> toggles_;
};

/** Load actions from file.
@param map the action map to fill
@param filename the filename to load from
 */
void Load(InputActionMap* map, const std::string& filename);

}  // namespace input

}  // namespace euphoria

#endif  // EUPHORIA_INPUT_ACTIONMAP_H_
