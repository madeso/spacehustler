// Euphoria - Copyright (c) Gustav

/** @file
Classes for input handling.
 */

#ifndef EUPHORIA_INPUT_MOUSEDEF_H_
#define EUPHORIA_INPUT_MOUSEDEF_H_

#include <vector>

#include "euphoria/input-unitdef.h"
#include "euphoria/input-axis.h"
#include "euphoria/input-binddef.h"
#include "euphoria/input-key.h"

namespace Json {
class Value;
}

namespace input {
class InputActionMap;

/** Mouse definition.
 */
class MouseDef : public UnitDef {
 public:
  /** Constructor.
  @param data the data structure to load from
  @param map the input actions to use
   */
  MouseDef(const Json::Value& data, const InputActionMap& map);

  /** Create a active mouse interface.
  @param director the input director
  @returns the active mouse
   */
  std::shared_ptr<ActiveUnit> Create(InputDirector* director,
                                     BindMap* map) override;

 private:
  std::vector<BindDef<Axis>> axis_;
  std::vector<BindDef<MouseButton::Type>> keys_;
};
}  // namespace input

#endif  // EUPHORIA_INPUT_MOUSEDEF_H_
