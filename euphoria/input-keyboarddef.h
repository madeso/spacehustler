// Euphoria - Copyright (c) Gustav

/** @file
Classes for input handling.
 */

#ifndef EUPHORIA_INPUT_KEYBOARDDEF_H_
#define EUPHORIA_INPUT_KEYBOARDDEF_H_

#include <vector>
#include <memory>

#include "euphoria/key.h"
#include "euphoria/input-bind.h"
#include "euphoria/input-axiskeybind.h"
#include "euphoria/input-unitdef.h"

namespace Json {
class Value;
}

namespace input {
class InputActionMap;
class ActiveUnit;
class InputDirector;

class KeyboardDef : public UnitDef {
 public:
  explicit KeyboardDef(const Json::Value& data, const InputActionMap& map);
  std::shared_ptr<ActiveUnit> Create(InputDirector* director);

 private:
  std::vector<Bind<Key::Type>> binds_;
  std::vector<AxisKeyBind<Key::Type>> axiskeys_;
};

}  // namespace input

#endif  // EUPHORIA_INPUT_KEYBOARDDEF_H_
