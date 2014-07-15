// Euphoria - Copyright (c) Gustav

/** @file
Classes for input handling.
 */

#ifndef EUPHORIA_INPUT_KEYCONFIG_H_
#define EUPHORIA_INPUT_KEYCONFIG_H_

#include <vector>
#include <memory>

#include "euphoria/input-connectedunits.h"

namespace input {

class UnitDef;
class InputDirector;
class InputActionMap;
class BindMap;

/** Contains a list of configurations.
A good example is Mouse+Keyboard.
 */
class KeyConfig {
 public:
  /** Constructor.
   */
  KeyConfig();

  /** Add a definition.
  @param def the definition to add.
   */
  void Add(std::shared_ptr<UnitDef> def);

  /** Connect.
  @param director the input director
  @returns the connected units
   */
  ConnectedUnits Connect(const InputActionMap& actions,
                         InputDirector* director);

 private:
  std::vector<std::shared_ptr<UnitDef>> definitions_;
  std::shared_ptr<BindMap> binds_;
};

}  // namespace input

#endif  // EUPHORIA_INPUT_KEYCONFIG_H_
