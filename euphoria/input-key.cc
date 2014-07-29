// Euphoria - Copyright (c) Gustav

#include "euphoria/input-key.h"

#include <string>
#include <map>

#include "euphoria/stringutils.h"

namespace {

template <typename T>
class Keyname {
 public:
  Keyname(T unbound, T invalid) : invalid_(invalid), unbound_(unbound) {
    (*this)(invalid_, "<invalid>")(unbound_, "<unbound>");
  }

  Keyname& operator()(T key, const std::string& name) {
    strtokey.insert(std::make_pair(ToLower(name), key));
    keytostr.insert(std::make_pair(key, name));

    return *this;
  }

  std::string fromKey(T key) const {
    auto r = keytostr.find(key);
    if (r == keytostr.end()) {
      return "Unknown";
    } else {
      return r->second;
    }
  }

  T fromString(const std::string& keyname) const {
    if (keyname == "") {
      return unbound_;
    }
    auto r = strtokey.find(ToLower(keyname));
    if (r == strtokey.end()) {
      return invalid_;
    } else {
      return r->second;
    }
  }

  std::map<std::string, T> strtokey;
  std::map<T, std::string> keytostr;
  const T invalid_;
  const T unbound_;
};

const Keyname<Key::Type>& AllKeys() {
  static Keyname<Key::Type> names = Keyname<Key::Type>(
      Key::Unbound, Key::Invalid)(Key::Return, "Return")(Key::Escape, "Escape")(
      Key::Backspace, "Backspace")(Key::Tab, "Tab")(Key::Space, "Space")(
      Key::Exclaim, "Exclaim")(Key::Quotedbl, "Quotedbl")(Key::Hash, "Hash")(
      Key::Percent, "Percent")(Key::Dollar, "Dollar")(
      Key::Ampersand, "Ampersand")(Key::Quote, "Quote")(
      Key::Leftparen, "Leftparen")(Key::Rightparen, "Rightparen")(
      Key::Asterisk, "Asterisk")(Key::Plus, "Plus")(Key::Comma, "Comma")(
      Key::Minus, "Minus")(Key::Period, "Period")(Key::Slash, "Slash")(
      Key::Num0, "Num0")(Key::Num1, "Num1")(Key::Num2, "Num2")(
      Key::Num3, "Num3")(Key::Num4, "Num4")(Key::Num5, "Num5")(
      Key::Num6, "Num6")(Key::Num7, "Num7")(Key::Num8, "Num8")(
      Key::Num9, "Num9")(Key::Colon, "Colon")(Key::Semicolon, "Semicolon")(
      Key::Less, "Less")(Key::Equals, "Equals")(Key::Greater, "Greater")(
      Key::Question, "Question")(Key::At, "At")(
      Key::Leftbracket, "Leftbracket")(Key::Backslash, "Backslash")(
      Key::Rightbracket, "Rightbracket")(Key::Caret, "Caret")(
      Key::Underscore, "Underscore")(Key::Backquote, "Backquote")(Key::A, "A")(
      Key::B, "B")(Key::C, "C")(Key::D, "D")(Key::E, "E")(Key::F, "F")(
      Key::G, "G")(Key::H, "H")(Key::I, "I")(Key::J, "J")(Key::K, "K")(
      Key::L, "L")(Key::M, "M")(Key::N, "N")(Key::O, "O")(Key::P, "P")(
      Key::Q, "Q")(Key::R, "R")(Key::S, "S")(Key::T, "T")(Key::U, "U")(
      Key::V, "V")(Key::W, "W")(Key::X, "X")(Key::Y, "Y")(Key::Z, "Z")(
      Key::Capslock, "Capslock")(Key::F1, "F1")(Key::F2, "F2")(Key::F3, "F3")(
      Key::F4, "F4")(Key::F5, "F5")(Key::F6, "F6")(Key::F7, "F7")(
      Key::F8, "F8")(Key::F9, "F9")(Key::F10, "F10")(Key::F11, "F11")(
      Key::F12, "F12")(Key::Printscreen, "Printscreen")(
      Key::Scrolllock, "Scrolllock")(Key::Pause, "Pause")(
      Key::Insert, "Insert")(Key::Home, "Home")(Key::Pageup, "Pageup")(
      Key::Delete, "Delete")(Key::End, "End")(Key::Pagedown, "Pagedown")(
      Key::Right, "Right")(Key::Left, "Left")(Key::Down, "Down")(Key::Up, "Up")(
      Key::Numlockclear, "Numlockclear")(Key::Kp_Divide, "Kp_Divide")(
      Key::Kp_Multiply, "Kp_Multiply")(Key::Kp_Minus, "Kp_Minus")(
      Key::Kp_Plus, "Kp_Plus")(Key::Kp_Enter, "Kp_Enter")(Key::Kp_1, "Kp_1")(
      Key::Kp_2, "Kp_2")(Key::Kp_3, "Kp_3")(Key::Kp_4, "Kp_4")(
      Key::Kp_5, "Kp_5")(Key::Kp_6, "Kp_6")(Key::Kp_7, "Kp_7")(
      Key::Kp_8, "Kp_8")(Key::Kp_9, "Kp_9")(Key::Kp_0, "Kp_0")(
      Key::Kp_Period, "Kp_Period")(Key::Application, "Application")(
      Key::Power, "Power")(Key::Kp_Equals, "Kp_Equals")(Key::F13, "F13")(
      Key::F14, "F14")(Key::F15, "F15")(Key::F16, "F16")(Key::F17, "F17")(
      Key::F18, "F18")(Key::F19, "F19")(Key::F20, "F20")(Key::F21, "F21")(
      Key::F22, "F22")(Key::F23, "F23")(Key::F24, "F24")(
      Key::Execute, "Execute")(Key::Help, "Help")(Key::Menu, "Menu")(
      Key::Select, "Select")(Key::Stop, "Stop")(Key::Again, "Again")(
      Key::Undo, "Undo")(Key::Cut, "Cut")(Key::Copy, "Copy")(
      Key::Paste, "Paste")(Key::Find, "Find")(Key::Mute, "Mute")(
      Key::Volumeup, "Volumeup")(Key::Volumedown, "Volumedown")(
      Key::Kp_Comma, "Kp_Comma")(Key::Kp_Equalsas400, "Kp_Equalsas400")(
      Key::Alterase, "Alterase")(Key::Sysreq, "Sysreq")(Key::Cancel, "Cancel")(
      Key::Clear, "Clear")(Key::Prior, "Prior")(Key::Return2, "Return2")(
      Key::Separator, "Separator")(Key::Out, "Out")(Key::Oper, "Oper")(
      Key::Clearagain, "Clearagain")(Key::Crsel, "Crsel")(Key::Exsel, "Exsel")(
      Key::Kp_00, "Kp_00")(Key::Kp_000, "Kp_000")(
      Key::Thousandsseparator, "Thousandsseparator")(Key::Decimalseparator,
                                                     "Decimalseparator")(
      Key::Currencyunit, "Currencyunit")(Key::Currencysubunit,
                                         "Currencysubunit")(
      Key::Kp_Leftparen, "Kp_Leftparen")(Key::Kp_Rightparen, "Kp_Rightparen")(
      Key::Kp_Leftbrace, "Kp_Leftbrace")(Key::Kp_Rightbrace, "Kp_Rightbrace")(
      Key::Kp_Tab, "Kp_Tab")(Key::Kp_Backspace, "Kp_Backspace")(
      Key::Kp_A, "Kp_A")(Key::Kp_B, "Kp_B")(Key::Kp_C, "Kp_C")(
      Key::Kp_D, "Kp_D")(Key::Kp_E, "Kp_E")(Key::Kp_F, "Kp_F")(
      Key::Kp_Xor, "Kp_Xor")(Key::Kp_Power, "Kp_Power")(
      Key::Kp_Percent, "Kp_Percent")(Key::Kp_Less, "Kp_Less")(
      Key::Kp_Greater, "Kp_Greater")(Key::Kp_Ampersand, "Kp_Ampersand")(
      Key::Kp_Dblampersand, "Kp_Dblampersand")(Key::Kp_Verticalbar,
                                               "Kp_Verticalbar")(
      Key::Kp_Dblverticalbar, "Kp_Dblverticalbar")(Key::Kp_Colon, "Kp_Colon")(
      Key::Kp_Hash, "Kp_Hash")(Key::Kp_Space, "Kp_Space")(Key::Kp_At, "Kp_At")(
      Key::Kp_Exclam, "Kp_Exclam")(Key::Kp_Memstore, "Kp_Memstore")(
      Key::Kp_Memrecall, "Kp_Memrecall")(Key::Kp_Memclear, "Kp_Memclear")(
      Key::Kp_Memadd, "Kp_Memadd")(Key::Kp_Memsubtract, "Kp_Memsubtract")(
      Key::Kp_Memmultiply, "Kp_Memmultiply")(Key::Kp_Memdivide, "Kp_Memdivide")(
      Key::Kp_Plusminus, "Kp_Plusminus")(Key::Kp_Clear, "Kp_Clear")(
      Key::Kp_Clearentry, "Kp_Clearentry")(Key::Kp_Binary, "Kp_Binary")(
      Key::Kp_Octal, "Kp_Octal")(Key::Kp_Decimal, "Kp_Decimal")(
      Key::Kp_Hexadecimal, "Kp_Hexadecimal")(Key::Lctrl, "Lctrl")(
      Key::Lshift, "Lshift")(Key::Lalt, "Lalt")(Key::Lgui, "Lgui")(
      Key::Rctrl, "Rctrl")(Key::Rshift, "Rshift")(Key::Ralt, "Ralt")(
      Key::Rgui, "Rgui")(Key::Mode, "Mode")(Key::Audionext, "Audionext")(
      Key::Audioprev, "Audioprev")(Key::Audiostop, "Audiostop")(
      Key::Audioplay, "Audioplay")(Key::Audiomute, "Audiomute")(
      Key::Mediaselect, "Mediaselect")(Key::Www, "Www")(Key::Mail, "Mail")(
      Key::Calculator, "Calculator")(Key::Computer, "Computer")(
      Key::Ac_Search, "Ac_Search")(Key::Ac_Home, "Ac_Home")(
      Key::Ac_Back, "Ac_Back")(Key::Ac_Forward, "Ac_Forward")(
      Key::Ac_Stop, "Ac_Stop")(Key::Ac_Refresh, "Ac_Refresh")(
      Key::Ac_Bookmarks, "Ac_Bookmarks")(Key::Brightnessdown, "Brightnessdown")(
      Key::Brightnessup, "Brightnessup")(Key::Displayswitch, "Displayswitch")(
      Key::Kbdillumtoggle, "Kbdillumtoggle")(Key::Kbdillumdown, "Kbdillumdown")(
      Key::Kbdillumup, "Kbdillumup")(Key::Eject, "Eject")(Key::Sleep, "Sleep");
  return names;
}

const Keyname<MouseButton>& AllMouseButtons() {
  static Keyname<MouseButton> buttons = Keyname<MouseButton>(
      MouseButton::Unbound, MouseButton::Invalid)(MouseButton::Left, "left")(
      MouseButton::Middle, "middle")(MouseButton::Right, "right")(
      MouseButton::X1, "x1")(MouseButton::X2, "x2");
  return buttons;
}

}  // namespace

namespace Key {
std::string ToString(Type k) { return AllKeys().fromKey(k); }

Type FromString(const std::string& keyname) {
  return AllKeys().fromString(keyname);
}
}  // namespace Key

std::string ToString(MouseButton k) { return AllMouseButtons().fromKey(k); }

MouseButton ToMouseButton(const std::string& keyname) {
  return AllMouseButtons().fromString(keyname);
}
