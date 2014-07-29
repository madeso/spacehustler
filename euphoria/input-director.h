// Euphoria - Copyright (c) Gustav

/** @file
Classes for input handling.
 */

#ifndef EUPHORIA_INPUT_DIRECTOR_H_
#define EUPHORIA_INPUT_DIRECTOR_H_

#include <vector>

#include "euphoria/input-key.h"
#include "euphoria/input-axis.h"

namespace input {

class KeyboardActiveUnit;
class MouseActiveUnit;
class JoystickActiveUnit;

/** Container class for directing input.
 */
class InputDirector {
 public:
  /** Constructor.
   */
  InputDirector();

  /** Destructor.
   */
  ~InputDirector();

  /** Add a keyboard.
  @param kb the keyboard
   */
  void Add(KeyboardActiveUnit* kb);

  /** Remove a keyboard.
  @param kb the keyboard
   */
  void Remove(KeyboardActiveUnit* kb);

  /** Add a mouse.
  @param au the mouse
  */
  void Add(MouseActiveUnit* au);

  /** Remove a mouse.
  @param au the mouse
  */
  void Remove(MouseActiveUnit* au);

  /** Add a joystick.
  @param au the joystick
  */
  void Add(JoystickActiveUnit* au);

  /** Remove a joystick.
  @param au the joystick
  */
  void Remove(JoystickActiveUnit* au);

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
  void OnMouseButton(MouseButton button, bool down);

  /** Send a joystick pov.
  @param type the pov type
  @param hat the pov hat
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

 private:
  std::vector<KeyboardActiveUnit*> keyboards_;
  std::vector<MouseActiveUnit*> mouse_;
  std::vector<JoystickActiveUnit*> joystick_;
};

}  // namespace input

#endif  // EUPHORIA_INPUT_DIRECTOR_H_
