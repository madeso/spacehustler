  // Euphoria - Copyright (c) Gustav
// Euphoria is awesome: 幸福感是真棒

/** @file
Key related code
 */

#ifndef EUPHORIA_KEY_H_
#define EUPHORIA_KEY_H_

#include <string>

namespace MouseButton {
  /** The mouse button
   */
  enum Type {
    Invalid                 /// Invalid key
    , Unbound               /// No key
    , Left                  /// The left mouse button
    , Middle                /// The middle mouse button
    , Right                 /// The right mouse button
    , X1                    /// The X2 mouse button
    , X2                    /// The X2 mouse button
  };
}  // namespace MouseButton

namespace Key {

  /** A key code
   */
  enum Type {
    Invalid                 /// Invalid key
    , Unbound               /// No key
    , Return              /// The key for Return
    , Escape              /// The key for Escape
    , Backspace           /// The key for Backspace
    , Tab                 /// The key for Tab
    , Space               /// The key for Space
    , Exclaim             /// The key for Exclaim
    , Quotedbl            /// The key for Quotedbl
    , Hash                /// The key for Hash
    , Percent             /// The key for Percent
    , Dollar              /// The key for Dollar
    , Ampersand           /// The key for Ampersand
    , Quote               /// The key for Quote
    , Leftparen           /// The key for Leftparen
    , Rightparen          /// The key for Rightparen
    , Asterisk            /// The key for Asterisk
    , Plus                /// The key for Plus
    , Comma               /// The key for Comma
    , Minus               /// The key for Minus
    , Period              /// The key for Period
    , Slash               /// The key for Slash
    , Num0                /// The key for Num0
    , Num1                /// The key for Num1
    , Num2                /// The key for Num2
    , Num3                /// The key for Num3
    , Num4                /// The key for Num4
    , Num5                /// The key for Num5
    , Num6                /// The key for Num6
    , Num7                /// The key for Num7
    , Num8                /// The key for Num8
    , Num9                /// The key for Num9
    , Colon               /// The key for Colon
    , Semicolon           /// The key for Semicolon
    , Less                /// The key for Less
    , Equals              /// The key for Equals
    , Greater             /// The key for Greater
    , Question            /// The key for Question
    , At                  /// The key for At
    , Leftbracket         /// The key for Leftbracket
    , Backslash           /// The key for Backslash
    , Rightbracket        /// The key for Rightbracket
    , Caret               /// The key for Caret
    , Underscore          /// The key for Underscore
    , Backquote           /// The key for Backquote
    , A                   /// The key for A
    , B                   /// The key for B
    , C                   /// The key for C
    , D                   /// The key for D
    , E                   /// The key for E
    , F                   /// The key for F
    , G                   /// The key for G
    , H                   /// The key for H
    , I                   /// The key for I
    , J                   /// The key for J
    , K                   /// The key for K
    , L                   /// The key for L
    , M                   /// The key for M
    , N                   /// The key for N
    , O                   /// The key for O
    , P                   /// The key for P
    , Q                   /// The key for Q
    , R                   /// The key for R
    , S                   /// The key for S
    , T                   /// The key for T
    , U                   /// The key for U
    , V                   /// The key for V
    , W                   /// The key for W
    , X                   /// The key for X
    , Y                   /// The key for Y
    , Z                   /// The key for Z
    , Capslock            /// The key for Capslock
    , F1                  /// The key for F1
    , F2                  /// The key for F2
    , F3                  /// The key for F3
    , F4                  /// The key for F4
    , F5                  /// The key for F5
    , F6                  /// The key for F6
    , F7                  /// The key for F7
    , F8                  /// The key for F8
    , F9                  /// The key for F9
    , F10                 /// The key for F10
    , F11                 /// The key for F11
    , F12                 /// The key for F12
    , Printscreen         /// The key for Printscreen
    , Scrolllock          /// The key for Scrolllock
    , Pause               /// The key for Pause
    , Insert              /// The key for Insert
    , Home                /// The key for Home
    , Pageup              /// The key for Pageup
    , Delete              /// The key for Delete
    , End                 /// The key for End
    , Pagedown            /// The key for Pagedown
    , Right               /// The key for Right
    , Left                /// The key for Left
    , Down                /// The key for Down
    , Up                  /// The key for Up
    , Numlockclear        /// The key for Numlockclear
    , Kp_Divide           /// The key for Kp_Divide
    , Kp_Multiply         /// The key for Kp_Multiply
    , Kp_Minus            /// The key for Kp_Minus
    , Kp_Plus             /// The key for Kp_Plus
    , Kp_Enter            /// The key for Kp_Enter
    , Kp_1                /// The key for Kp_1
    , Kp_2                /// The key for Kp_2
    , Kp_3                /// The key for Kp_3
    , Kp_4                /// The key for Kp_4
    , Kp_5                /// The key for Kp_5
    , Kp_6                /// The key for Kp_6
    , Kp_7                /// The key for Kp_7
    , Kp_8                /// The key for Kp_8
    , Kp_9                /// The key for Kp_9
    , Kp_0                /// The key for Kp_0
    , Kp_Period           /// The key for Kp_Period
    , Application         /// The key for Application
    , Power               /// The key for Power
    , Kp_Equals           /// The key for Kp_Equals
    , F13                 /// The key for F13
    , F14                 /// The key for F14
    , F15                 /// The key for F15
    , F16                 /// The key for F16
    , F17                 /// The key for F17
    , F18                 /// The key for F18
    , F19                 /// The key for F19
    , F20                 /// The key for F20
    , F21                 /// The key for F21
    , F22                 /// The key for F22
    , F23                 /// The key for F23
    , F24                 /// The key for F24
    , Execute             /// The key for Execute
    , Help                /// The key for Help
    , Menu                /// The key for Menu
    , Select              /// The key for Select
    , Stop                /// The key for Stop
    , Again               /// The key for Again
    , Undo                /// The key for Undo
    , Cut                 /// The key for Cut
    , Copy                /// The key for Copy
    , Paste               /// The key for Paste
    , Find                /// The key for Find
    , Mute                /// The key for Mute
    , Volumeup            /// The key for Volumeup
    , Volumedown          /// The key for Volumedown
    , Kp_Comma            /// The key for Kp_Comma
    , Kp_Equalsas400      /// The key for Kp_Equalsas400
    , Alterase            /// The key for Alterase
    , Sysreq              /// The key for Sysreq
    , Cancel              /// The key for Cancel
    , Clear               /// The key for Clear
    , Prior               /// The key for Prior
    , Return2             /// The key for Return2
    , Separator           /// The key for Separator
    , Out                 /// The key for Out
    , Oper                /// The key for Oper
    , Clearagain          /// The key for Clearagain
    , Crsel               /// The key for Crsel
    , Exsel               /// The key for Exsel
    , Kp_00               /// The key for Kp_00
    , Kp_000              /// The key for Kp_000
    , Thousandsseparator  /// The key for Thousandsseparator
    , Decimalseparator    /// The key for Decimalseparator
    , Currencyunit        /// The key for Currencyunit
    , Currencysubunit     /// The key for Currencysubunit
    , Kp_Leftparen        /// The key for Kp_Leftparen
    , Kp_Rightparen       /// The key for Kp_Rightparen
    , Kp_Leftbrace        /// The key for Kp_Leftbrace
    , Kp_Rightbrace       /// The key for Kp_Rightbrace
    , Kp_Tab              /// The key for Kp_Tab
    , Kp_Backspace        /// The key for Kp_Backspace
    , Kp_A                /// The key for Kp_A
    , Kp_B                /// The key for Kp_B
    , Kp_C                /// The key for Kp_C
    , Kp_D                /// The key for Kp_D
    , Kp_E                /// The key for Kp_E
    , Kp_F                /// The key for Kp_F
    , Kp_Xor              /// The key for Kp_Xor
    , Kp_Power            /// The key for Kp_Power
    , Kp_Percent          /// The key for Kp_Percent
    , Kp_Less             /// The key for Kp_Less
    , Kp_Greater          /// The key for Kp_Greater
    , Kp_Ampersand        /// The key for Kp_Ampersand
    , Kp_Dblampersand     /// The key for Kp_Dblampersand
    , Kp_Verticalbar      /// The key for Kp_Verticalbar
    , Kp_Dblverticalbar   /// The key for Kp_Dblverticalbar
    , Kp_Colon            /// The key for Kp_Colon
    , Kp_Hash             /// The key for Kp_Hash
    , Kp_Space            /// The key for Kp_Space
    , Kp_At               /// The key for Kp_At
    , Kp_Exclam           /// The key for Kp_Exclam
    , Kp_Memstore         /// The key for Kp_Memstore
    , Kp_Memrecall        /// The key for Kp_Memrecall
    , Kp_Memclear         /// The key for Kp_Memclear
    , Kp_Memadd           /// The key for Kp_Memadd
    , Kp_Memsubtract      /// The key for Kp_Memsubtract
    , Kp_Memmultiply      /// The key for Kp_Memmultiply
    , Kp_Memdivide        /// The key for Kp_Memdivide
    , Kp_Plusminus        /// The key for Kp_Plusminus
    , Kp_Clear            /// The key for Kp_Clear
    , Kp_Clearentry       /// The key for Kp_Clearentry
    , Kp_Binary           /// The key for Kp_Binary
    , Kp_Octal            /// The key for Kp_Octal
    , Kp_Decimal          /// The key for Kp_Decimal
    , Kp_Hexadecimal      /// The key for Kp_Hexadecimal
    , Lctrl               /// The key for Lctrl
    , Lshift              /// The key for Lshift
    , Lalt                /// The key for Lalt
    , Lgui                /// The key for Lgui
    , Rctrl               /// The key for Rctrl
    , Rshift              /// The key for Rshift
    , Ralt                /// The key for Ralt
    , Rgui                /// The key for Rgui
    , Mode                /// The key for Mode
    , Audionext           /// The key for Audionext
    , Audioprev           /// The key for Audioprev
    , Audiostop           /// The key for Audiostop
    , Audioplay           /// The key for Audioplay
    , Audiomute           /// The key for Audiomute
    , Mediaselect         /// The key for Mediaselect
    , Www                 /// The key for Www
    , Mail                /// The key for Mail
    , Calculator          /// The key for Calculator
    , Computer            /// The key for Computer
    , Ac_Search           /// The key for Ac_Search
    , Ac_Home             /// The key for Ac_Home
    , Ac_Back             /// The key for Ac_Back
    , Ac_Forward          /// The key for Ac_Forward
    , Ac_Stop             /// The key for Ac_Stop
    , Ac_Refresh          /// The key for Ac_Refresh
    , Ac_Bookmarks        /// The key for Ac_Bookmarks
    , Brightnessdown      /// The key for Brightnessdown
    , Brightnessup        /// The key for Brightnessup
    , Displayswitch       /// The key for Displayswitch
    , Kbdillumtoggle      /// The key for Kbdillumtoggle
    , Kbdillumdown        /// The key for Kbdillumdown
    , Kbdillumup          /// The key for Kbdillumup
    , Eject               /// The key for Eject
    , Sleep               /// The key for Sleep
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
