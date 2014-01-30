// Euphoria - Copyright (c) Gustav

/** @file
Classes for input handling.
 */

#ifndef EUPHORIA_INPUTSYSTEM_H_
#define EUPHORIA_INPUTSYSTEM_H_

#include <vector>
#include <memory>

#include "euphoria/input-connectedunits.h"

namespace input {

  class UnitDef;
  class InputDirector;

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
  ConnectedUnits Connect(InputDirector* director) const;

 private:
  std::vector<std::shared_ptr<UnitDef>> definitions_;
};

}  // namespace input

#endif  // EUPHORIA_INPUTSYSTEM_H_
