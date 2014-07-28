// Euphoria - Copyright (c) Gustav

/** @file
Classes for input handling.
 */

#ifndef EUPHORIA_INPUT_SYSTEM_H_
#define EUPHORIA_INPUT_SYSTEM_H_

#include <vector>
#include <map>
#include <string>
#include <memory>

#include "euphoria/input-key.h"
#include "euphoria/input-axis.h"
#include "euphoria/input-keyconfigs.h"
#include "euphoria/input-actionmap.h"

class Table;

namespace input {

class Player;
class GlobalToggle;
class InputDirector;

/** the master class that controls the input system.
 */
class InputSystem {
 public:
  /** Constructor.
   */
  InputSystem();

  /** Destructor.
   */
  ~InputSystem();

  /** Get a action.
  @param name the name of the action
  @returns the action
   */
  std::shared_ptr<GlobalToggle> GetAction(const std::string& name);

  /** Setups a input unit for a player.
  @param playerName the name of the player
  @param inputName the name of the input
   */
  void SetUnitForPlayer(const std::string& playerName,
                        const std::string& inputName);

  void Update(float dt);

  /** Send a keyboard event.
  @param key the key to act upon
  @param down true if it is down, false if not
   */
  void OnKeyboardKey(Key::Type key, bool down);

  /** Send a mouse axis.
  @param axis the axis
  @param value the value
   */
  void OnMouseAxis(Axis axis, float value);

  /** Send a mouse button.
  @param button the button to act upon
  @param down true if it is down, false if not
   */
  void OnMouseButton(MouseButton::Type button, bool down);

  /** Send a joystick pov.
  @param type the pov type
  @param hat the joystick pov hat
  @param joystick the joystick id
  @param value the value
   */
  void OnJoystickPov(Axis type, int hat, int joystick, float value);

  /** Send a joystick button.
  @param button the button to act upon
  @param joystick the joystick id
  @param down true if it is down, false if not
   */
  void OnJoystickButton(int button, int joystick, bool down);

  /** Send a joystick axis.
  @param axis the axis
  @param joystick the joystick id
  @param value the value
   */
  void OnJoystickAxis(int axis, int joystick, float value);

  /** Get a player.
  @param name the name of the player
  @returns the player
   */
  std::shared_ptr<Player> GetPlayer(const std::string& name);

  /** Add a player.
  @name the name of the player.
   */
  void AddPlayer(const std::string& name);

 private:
  InputActionMap actions_;
  std::map<std::string, std::shared_ptr<Player>> players_;
  KeyConfigs configs_;
  std::unique_ptr<InputDirector> input_;
};

}  // namespace input

#endif  // EUPHORIA_INPUT_SYSTEM_H_
