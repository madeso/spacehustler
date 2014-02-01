// Euphoria - Copyright (c) Gustav

/** @file
Classes for input handling.
 */

#ifndef EUPHORIA_INPUT_KEYBOARDACTIVEUNIT_H_
#define EUPHORIA_INPUT_KEYBOARDACTIVEUNIT_H_

#include <vector>
#include <map>
#include <memory>

#include "euphoria/input-activeunit.h"
#include "euphoria/input-axiskeybind.h"
#include "euphoria/input-bind.h"

#include "euphoria/input-key.h"

namespace input {

class AxisKey;
class InputDirector;

/** A active keyboard.
 */
class KeyboardActiveUnit : public ActiveUnit {
 public:
  /** Constructor.
  @param binds the key binds
  @param axiskeys the axis binds
  @param director the director
   */
  KeyboardActiveUnit(const std::vector<Bind<Key::Type>>& binds,
                     std::vector<AxisKeyBind<Key::Type>> axiskeys,
                     InputDirector* director);

  /** On key handler.
  @param key the key
  @param state the state of the button
   */
  void OnKey(const Key::Type& key, bool state);

  /** Destructor.
   */
  ~KeyboardActiveUnit();

  /** Rumble function, not really useful.
   */
  void Rumble();

 private:
  InputDirector* director_;
  std::map<Key::Type, std::shared_ptr<InputAction>> actions_;
  std::map<Key::Type, std::shared_ptr<AxisKey>> axiskeys_;
};

}  // namespace input

#endif  // EUPHORIA_INPUT_KEYBOARDACTIVEUNIT_H_
