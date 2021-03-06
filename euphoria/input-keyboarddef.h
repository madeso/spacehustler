// Euphoria - Copyright (c) Gustav

/** @file
Classes for input handling.
 */

#ifndef EUPHORIA_INPUT_KEYBOARDDEF_H_
#define EUPHORIA_INPUT_KEYBOARDDEF_H_

#include <vector>
#include <memory>

#include "euphoria/input-key.h"
#include "euphoria/input-unitdef.h"
#include "euphoria/input-binddef.h"

namespace Json {
class Value;
}

namespace euphoria {

namespace input {
class InputActionMap;
class ActiveUnit;
class InputDirector;

/** Keyboard definition.
 */
class KeyboardDef : public UnitDef {
 public:
  /** Constructor.
  @param data the data structure to load from
  @param map the input actions to use
   */
  KeyboardDef(const Json::Value& data, const InputActionMap& map);

  /** Create a active keyboard interface.
  @param director the input director
  @returns the active keyboard
   */
  std::shared_ptr<ActiveUnit> Create(InputDirector* director,
                                     BindMap* map) override;

 private:
  std::vector<BindDef<Key>> keys_;
};

}  // namespace input

}  // namespace euphoria

#endif  // EUPHORIA_INPUT_KEYBOARDDEF_H_
