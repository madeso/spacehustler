  // Euphoria - Copyright (c) Gustav
// Euphoria is awesome: 幸福感是真棒

/** @file
Key related code
 */

#ifndef EUPHORIA_KEY_H_
#define EUPHORIA_KEY_H_

#include <string>

namespace Key {

  /** A key code
   */
  enum Type {
    Invalid                 /// Invalid key
    , Unbound               /// No key
    , A                     /// The A key
    , B                     /// The B key
    , C                     /// The C key
    , D                     /// The D key
    , E                     /// The E key
    , F                     /// The F key
    , G                     /// The G key
    , H                     /// The H key
    , I                     /// The I key
    , J                     /// The J key
    , K                     /// The K key
    , L                     /// The L key
    , M                     /// The M key
    , N                     /// The N key
    , O                     /// The O key
    , P                     /// The P key
    , Q                     /// The Q key
    , R                     /// The R key
    , S                     /// The S key
    , T                     /// The T key
    , U                     /// The U key
    , V                     /// The V key
    , W                     /// The W key
    , X                     /// The X key
    , Y                     /// The Y key
    , Z                     /// The Z key
    , Num0                  /// The 0 key
    , Num1                  /// The 1 key
    , Num2                  /// The 2 key
    , Num3                  /// The 3 key
    , Num4                  /// The 4 key
    , Num5                  /// The 5 key
    , Num6                  /// The 6 key
    , Num7                  /// The 7 key
    , Num8                  /// The 8 key
    , Num9                  /// The 9 key
    , Escape                /// The Escape key
    , LControl              /// The left Control key
    , LShift                /// The left Shift key
    , LAlt                  /// The left Alt key
    , LSystem               /// The left OS specific key
    , RControl              /// The right Control key
    , RShift                /// The right Shift key
    , RAlt                  /// The right Alt key
    , RSystem               /// The right OS specific key
    , Menu                  /// The Menu key
    , LBracket              /// The [ key
    , RBracket              /// The ] key
    , SemiColon             /// The ; key
    , Comma                 /// The  key
    , Period                /// The . key
    , Quote                 /// The ' key
    , Slash                 /// The / key
    , BackSlash             /// The \ key
    , Tilde                 /// The ~ key
    , Equal                 /// The = key
    , Dash                  /// The - key
    , Space                 /// The Space key
    , Return                /// The Return key
    , BackSpace             /// The Backspace key
    , Tab                   /// The Tabulation key
    , PageUp                /// The Page up key
    , PageDown              /// The Page down key
    , End                   /// The End key
    , Home                  /// The Home key
    , Insert                /// The Insert key
    , Delete                /// The Delete key
    , Add                   /// The + key
    , Subtract              /// The - key
    , Multiply              /// The * key
    , Divide                /// The / key
    , Left                  /// Left arrow
    , Right                 /// Right arrow
    , Up                    /// Up arrow
    , Down                  /// Down arrow
    , Numpad0               /// The numpad 0 key
    , Numpad1               /// The numpad 1 key
    , Numpad2               /// The numpad 2 key
    , Numpad3               /// The numpad 3 key
    , Numpad4               /// The numpad 4 key
    , Numpad5               /// The numpad 5 key
    , Numpad6               /// The numpad 6 key
    , Numpad7               /// The numpad 7 key
    , Numpad8               /// The numpad 8 key
    , Numpad9               /// The numpad 9 key
    , F1                    /// The F1 key
    , F2                    /// The F2 key
    , F3                    /// The F3 key
    , F4                    /// The F4 key
    , F5                    /// The F5 key
    , F6                    /// The F6 key
    , F7                    /// The F7 key
    , F8                    /// The F8 key
    , F9                    /// The F8 key
    , F10                   /// The F10 key
    , F11                   /// The F11 key
    , F12                   /// The F12 key
    , F13                   /// The F13 key
    , F14                   /// The F14 key
    , F15                   /// The F15 key
    , Pause                 /// The Pause key
    , MouseLeft             /// The left mouse button
    , MouseRight            /// The right mouse button
    , MouseMiddle           /// The middle (wheel) mouse button
    , MouseXButton1         /// The first extra mouse button
    , MouseXButton2         /// The second extra mouse button
    , MouseXPositive        /// The mouse X+
    , MouseXNegative        /// The mouse X-
    , MouseYPositive        /// The mouse Y+
    , MouseYNegative        /// The mouse Y-
    , JoystickButton1       /// The joystick button #1
    , JoystickButton2       /// The joystick button #2
    , JoystickButton3       /// The joystick button #3
    , JoystickButton4       /// The joystick button #4
    , JoystickButton5       /// The joystick button #5
    , JoystickButton6       /// The joystick button #6
    , JoystickButton7       /// The joystick button #7
    , JoystickButton8       /// The joystick button #8
    , JoystickButton9       /// The joystick button #9
    , JoystickButton10      /// The joystick button #10
    , JoystickButton11      /// The joystick button #11
    , JoystickButton12      /// The joystick button #12
    , JoystickButton13      /// The joystick button #13
    , JoystickButton14      /// The joystick button #14
    , JoystickButton15      /// The joystick button #15
    , JoystickButton16      /// The joystick button #16
    , JoystickButton17      /// The joystick button #17
    , JoystickButton18      /// The joystick button #18
    , JoystickButton19      /// The joystick button #19
    , JoystickButton20      /// The joystick button #20
    , JoystickButton21      /// The joystick button #21
    , JoystickButton22      /// The joystick button #22
    , JoystickButton23      /// The joystick button #23
    , JoystickButton24      /// The joystick button #24
    , JoystickButton25      /// The joystick button #25
    , JoystickButton26      /// The joystick button #26
    , JoystickButton27      /// The joystick button #27
    , JoystickButton28      /// The joystick button #28
    , JoystickButton29      /// The joystick button #29
    , JoystickButton30      /// The joystick button #30
    , JoystickButton31      /// The joystick button #31
    , JoystickButton32      /// The joystick button #32
    , JoystickXPositive     /// Joystick X+
    , JoystickYPositive     /// Joystick Y+
    , JoystickZPositive     /// Joystick Z+
    , JoystickRPositive     /// Joystick R+
    , JoystickUPositive     /// Joystick U+
    , JoystickVPositive     /// Joystick V+
    , JoystickPovXPositive  /// Joystick PovX+
    , JoystickPovYPositive  /// Joystick PovY+
    , JoystickXNegative     /// Joystick X-
    , JoystickYNegative     /// Joystick Y-
    , JoystickZNegative     /// Joystick Z-
    , JoystickRNegative     /// Joystick R-
    , JoystickUNegative     /// Joystick U-
    , JoystickVNegative     /// Joystick V-
    , JoystickPovXNegative  /// Joystick PovX-
    , JoystickPovYNegative  /// Joystick PovY-
    , MouseX                /// Mouse axis X
    , MouseY                /// Mouse axis Y
    , JoystickX             /// Joystick axis X
    , JoystickY             /// Joystick axis Y
    , JoystickZ             /// Joystick axis Z
    , JoystickR             /// Joystick axis R
    , JoystickU             /// Joystick axis U
    , JoystickV             /// Joystick axis V
    , JoystickPovX          /// Joystick axis PovX
    , JoystickPovY          /// Joystick axis PovY
  };

  /** Generate a string representation of the given key
  @param k the key
  @returns the string representation
   */
  std::string ToString(Type k);

  /** Generate a key from a string.
  @param keyname the name of the key
  @returns the key or None
   */
  Type FromString(const std::string& keyname);
}  // namespace Key

#endif  // EUPHORIA_KEY_H_
