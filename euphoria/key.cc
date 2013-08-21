// Euphoria - Copyright (c) Gustav

#include "euphoria/key.h"

#include <string>
#include <map>

#include "euphoria/stringutils.h"

namespace {
  class Keyname {
    public:
      Keyname() {
        bind(Key::Invalid, "<invalid>");
        bind(Key::Unbound, "<unbound>");
        bind(Key::A, "A");
        bind(Key::B, "B");
        bind(Key::C, "C");
        bind(Key::D, "D");
        bind(Key::E, "E");
        bind(Key::F, "F");
        bind(Key::G, "G");
        bind(Key::H, "H");
        bind(Key::I, "I");
        bind(Key::J, "J");
        bind(Key::K, "K");
        bind(Key::L, "L");
        bind(Key::M, "M");
        bind(Key::N, "N");
        bind(Key::O, "O");
        bind(Key::P, "P");
        bind(Key::Q, "Q");
        bind(Key::R, "R");
        bind(Key::S, "S");
        bind(Key::T, "T");
        bind(Key::U, "U");
        bind(Key::V, "V");
        bind(Key::W, "W");
        bind(Key::X, "X");
        bind(Key::Y, "Y");
        bind(Key::Z, "Z");
        bind(Key::Num0, "0");
        bind(Key::Num1, "1");
        bind(Key::Num2, "2");
        bind(Key::Num3, "3");
        bind(Key::Num4, "4");
        bind(Key::Num5, "5");
        bind(Key::Num6, "6");
        bind(Key::Num7, "7");
        bind(Key::Num8, "8");
        bind(Key::Num9, "9");
        bind(Key::Escape, "Esc");
        bind(Key::LControl, "Left Control");
        bind(Key::LShift, "Left Shift");
        bind(Key::LAlt, "Left Alt");
        bind(Key::LSystem, "Left System");
        bind(Key::RControl, "Right Control");
        bind(Key::RShift, "Right Shift");
        bind(Key::RAlt, "Right Alt");
        bind(Key::RSystem, "Right System");
        bind(Key::Menu, "Menu");
        bind(Key::LBracket, "Left Bracket");
        bind(Key::RBracket, "Right Bracket");
        bind(Key::SemiColon, "SemiColon");
        bind(Key::Comma, "Comma");
        bind(Key::Period, "Period");
        bind(Key::Quote, "Quote");
        bind(Key::Slash, "Slash");
        bind(Key::BackSlash, "BackSlash");
        bind(Key::Tilde, "Tilde");
        bind(Key::Equal, "Equal");
        bind(Key::Dash, "Dash");
        bind(Key::Space, "Space");
        bind(Key::Return, "Return");
        bind(Key::BackSpace, "Backspace");
        bind(Key::Tab, "Tab");
        bind(Key::PageUp, "PageUp");
        bind(Key::PageDown, "PageDown");
        bind(Key::End, "End");
        bind(Key::Home, "Home");
        bind(Key::Insert, "Insert");
        bind(Key::Delete, "Delete");
        bind(Key::Add, "Add");
        bind(Key::Subtract, "Subtract");
        bind(Key::Multiply, "Multiply");
        bind(Key::Divide, "Divide");
        bind(Key::Left, "Left");
        bind(Key::Right, "Right");
        bind(Key::Up, "Up");
        bind(Key::Down, "Down");
        bind(Key::Numpad0, "Numpad 0");
        bind(Key::Numpad1, "Numpad 1");
        bind(Key::Numpad2, "Numpad 2");
        bind(Key::Numpad3, "Numpad 3");
        bind(Key::Numpad4, "Numpad 4");
        bind(Key::Numpad5, "Numpad 5");
        bind(Key::Numpad6, "Numpad 6");
        bind(Key::Numpad7, "Numpad 7");
        bind(Key::Numpad8, "Numpad 8");
        bind(Key::Numpad9, "Numpad 9");
        bind(Key::F1, "F1");
        bind(Key::F2, "F2");
        bind(Key::F3, "F3");
        bind(Key::F4, "F4");
        bind(Key::F5, "F5");
        bind(Key::F6, "F6");
        bind(Key::F7, "F7");
        bind(Key::F8, "F8");
        bind(Key::F9, "F9");
        bind(Key::F10, "F10");
        bind(Key::F11, "F11");
        bind(Key::F12, "F12");
        bind(Key::F13, "F13");
        bind(Key::F14, "F14");
        bind(Key::F15, "F15");
        bind(Key::Pause, "Pause");
        bind(Key::MouseLeft, "LMB");
        bind(Key::MouseRight, "RMB");
        bind(Key::MouseMiddle, "MMB");
        bind(Key::MouseXButton1, "MouseXButton1");
        bind(Key::MouseXButton2, "MouseXButton2");
        bind(Key::MouseXPositive, "MouseX+");
        bind(Key::MouseXNegative, "MouseX-");
        bind(Key::MouseYPositive, "MouseY+");
        bind(Key::MouseYNegative, "MouseY-");
        bind(Key::JoystickButton1 , "JoystickButton1");
        bind(Key::JoystickButton2 , "JoystickButton2");
        bind(Key::JoystickButton3 , "JoystickButton3");
        bind(Key::JoystickButton4 , "JoystickButton4");
        bind(Key::JoystickButton5 , "JoystickButton5");
        bind(Key::JoystickButton6 , "JoystickButton6");
        bind(Key::JoystickButton7 , "JoystickButton7");
        bind(Key::JoystickButton8 , "JoystickButton8");
        bind(Key::JoystickButton9 , "JoystickButton9");
        bind(Key::JoystickButton10, "JoystickButton10");
        bind(Key::JoystickButton11, "JoystickButton11");
        bind(Key::JoystickButton12, "JoystickButton12");
        bind(Key::JoystickButton13, "JoystickButton13");
        bind(Key::JoystickButton14, "JoystickButton14");
        bind(Key::JoystickButton15, "JoystickButton15");
        bind(Key::JoystickButton16, "JoystickButton16");
        bind(Key::JoystickButton17, "JoystickButton17");
        bind(Key::JoystickButton18, "JoystickButton18");
        bind(Key::JoystickButton19, "JoystickButton19");
        bind(Key::JoystickButton20, "JoystickButton20");
        bind(Key::JoystickButton21, "JoystickButton21");
        bind(Key::JoystickButton22, "JoystickButton22");
        bind(Key::JoystickButton23, "JoystickButton23");
        bind(Key::JoystickButton24, "JoystickButton24");
        bind(Key::JoystickButton25, "JoystickButton25");
        bind(Key::JoystickButton26, "JoystickButton26");
        bind(Key::JoystickButton27, "JoystickButton27");
        bind(Key::JoystickButton28, "JoystickButton28");
        bind(Key::JoystickButton29, "JoystickButton29");
        bind(Key::JoystickButton30, "JoystickButton30");
        bind(Key::JoystickButton31, "JoystickButton31");
        bind(Key::JoystickButton32, "JoystickButton32");
        bind(Key::JoystickXPositive, "JoystickX+");
        bind(Key::JoystickYPositive, "JoystickY+");
        bind(Key::JoystickZPositive, "JoystickZ+");
        bind(Key::JoystickRPositive, "JoystickR+");
        bind(Key::JoystickUPositive, "JoystickU+");
        bind(Key::JoystickVPositive, "JoystickV+");
        bind(Key::JoystickPovXPositive, "JoystickPovX+");
        bind(Key::JoystickPovYPositive, "JoystickPovY+");
        bind(Key::JoystickXNegative, "JoystickX-");
        bind(Key::JoystickYNegative, "JoystickY-");
        bind(Key::JoystickZNegative, "JoystickZ-");
        bind(Key::JoystickRNegative, "JoystickR-");
        bind(Key::JoystickUNegative, "JoystickU-");
        bind(Key::JoystickVNegative, "JoystickV-");
        bind(Key::JoystickPovXNegative, "JoystickPovX-");
        bind(Key::JoystickPovYNegative, "JoystickPovY-");
        bind(Key::MouseX, "MouseX");
        bind(Key::MouseY, "MouseY");
        bind(Key::JoystickX, "JoystickX");
        bind(Key::JoystickY, "JoystickY");
        bind(Key::JoystickZ, "JoystickZ");
        bind(Key::JoystickR, "JoystickR");
        bind(Key::JoystickU, "JoystickU");
        bind(Key::JoystickV, "JoystickV");
        bind(Key::JoystickPovX, "JoystickPovX");
        bind(Key::JoystickPovY, "JoystickPovY");
      }

      void bind(Key::Type key, const std::string name) {
        strtokey.insert(std::make_pair(ToLower(name), key));
        keytostr.insert(std::make_pair(key, name));
      }

      std::string fromKey(Key::Type key) const {
        auto r = keytostr.find(key);
        if (r == keytostr.end()) {
          return "Unknown";
        } else {
          return r->second;
        }
      }

      Key::Type fromString(const std::string& keyname) const {
        if (keyname == "") {
          return Key::Unbound;
        }
        auto r = strtokey.find(ToLower(keyname));
        if (r == strtokey.end()) {
          return Key::Invalid;
        } else {
          return r->second;
        }
      }

      std::map<std::string, Key::Type> strtokey;
      std::map<Key::Type, std::string> keytostr;
  };

  const Keyname& AllKeys() {
    static Keyname names;
    return names;
  }
}  // namespace

namespace Key {
  std::string ToString(Type k) {
    return AllKeys().fromKey(k);
  }

  Type FromString(const std::string& keyname) {
    return AllKeys().fromString(keyname);
  }
}  // namespace Key
