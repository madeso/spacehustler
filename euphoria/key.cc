  // Euphoria - Copyright (c) Gustav
// Euphoria is awesome: 幸福感是真棒

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

        bind(Key::Return, "Return");
        bind(Key::Escape, "Escape");
        bind(Key::Backspace, "Backspace");
        bind(Key::Tab, "Tab");
        bind(Key::Space, "Space");
        bind(Key::Exclaim, "Exclaim");
        bind(Key::Quotedbl, "Quotedbl");
        bind(Key::Hash, "Hash");
        bind(Key::Percent, "Percent");
        bind(Key::Dollar, "Dollar");
        bind(Key::Ampersand, "Ampersand");
        bind(Key::Quote, "Quote");
        bind(Key::Leftparen, "Leftparen");
        bind(Key::Rightparen, "Rightparen");
        bind(Key::Asterisk, "Asterisk");
        bind(Key::Plus, "Plus");
        bind(Key::Comma, "Comma");
        bind(Key::Minus, "Minus");
        bind(Key::Period, "Period");
        bind(Key::Slash, "Slash");
        bind(Key::Num0, "Num0");
        bind(Key::Num1, "Num1");
        bind(Key::Num2, "Num2");
        bind(Key::Num3, "Num3");
        bind(Key::Num4, "Num4");
        bind(Key::Num5, "Num5");
        bind(Key::Num6, "Num6");
        bind(Key::Num7, "Num7");
        bind(Key::Num8, "Num8");
        bind(Key::Num9, "Num9");
        bind(Key::Colon, "Colon");
        bind(Key::Semicolon, "Semicolon");
        bind(Key::Less, "Less");
        bind(Key::Equals, "Equals");
        bind(Key::Greater, "Greater");
        bind(Key::Question, "Question");
        bind(Key::At, "At");
        bind(Key::Leftbracket, "Leftbracket");
        bind(Key::Backslash, "Backslash");
        bind(Key::Rightbracket, "Rightbracket");
        bind(Key::Caret, "Caret");
        bind(Key::Underscore, "Underscore");
        bind(Key::Backquote, "Backquote");
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
        bind(Key::Capslock, "Capslock");
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
        bind(Key::Printscreen, "Printscreen");
        bind(Key::Scrolllock, "Scrolllock");
        bind(Key::Pause, "Pause");
        bind(Key::Insert, "Insert");
        bind(Key::Home, "Home");
        bind(Key::Pageup, "Pageup");
        bind(Key::Delete, "Delete");
        bind(Key::End, "End");
        bind(Key::Pagedown, "Pagedown");
        bind(Key::Right, "Right");
        bind(Key::Left, "Left");
        bind(Key::Down, "Down");
        bind(Key::Up, "Up");
        bind(Key::Numlockclear, "Numlockclear");
        bind(Key::Kp_Divide, "Kp_Divide");
        bind(Key::Kp_Multiply, "Kp_Multiply");
        bind(Key::Kp_Minus, "Kp_Minus");
        bind(Key::Kp_Plus, "Kp_Plus");
        bind(Key::Kp_Enter, "Kp_Enter");
        bind(Key::Kp_1, "Kp_1");
        bind(Key::Kp_2, "Kp_2");
        bind(Key::Kp_3, "Kp_3");
        bind(Key::Kp_4, "Kp_4");
        bind(Key::Kp_5, "Kp_5");
        bind(Key::Kp_6, "Kp_6");
        bind(Key::Kp_7, "Kp_7");
        bind(Key::Kp_8, "Kp_8");
        bind(Key::Kp_9, "Kp_9");
        bind(Key::Kp_0, "Kp_0");
        bind(Key::Kp_Period, "Kp_Period");
        bind(Key::Application, "Application");
        bind(Key::Power, "Power");
        bind(Key::Kp_Equals, "Kp_Equals");
        bind(Key::F13, "F13");
        bind(Key::F14, "F14");
        bind(Key::F15, "F15");
        bind(Key::F16, "F16");
        bind(Key::F17, "F17");
        bind(Key::F18, "F18");
        bind(Key::F19, "F19");
        bind(Key::F20, "F20");
        bind(Key::F21, "F21");
        bind(Key::F22, "F22");
        bind(Key::F23, "F23");
        bind(Key::F24, "F24");
        bind(Key::Execute, "Execute");
        bind(Key::Help, "Help");
        bind(Key::Menu, "Menu");
        bind(Key::Select, "Select");
        bind(Key::Stop, "Stop");
        bind(Key::Again, "Again");
        bind(Key::Undo, "Undo");
        bind(Key::Cut, "Cut");
        bind(Key::Copy, "Copy");
        bind(Key::Paste, "Paste");
        bind(Key::Find, "Find");
        bind(Key::Mute, "Mute");
        bind(Key::Volumeup, "Volumeup");
        bind(Key::Volumedown, "Volumedown");
        bind(Key::Kp_Comma, "Kp_Comma");
        bind(Key::Kp_Equalsas400, "Kp_Equalsas400");
        bind(Key::Alterase, "Alterase");
        bind(Key::Sysreq, "Sysreq");
        bind(Key::Cancel, "Cancel");
        bind(Key::Clear, "Clear");
        bind(Key::Prior, "Prior");
        bind(Key::Return2, "Return2");
        bind(Key::Separator, "Separator");
        bind(Key::Out, "Out");
        bind(Key::Oper, "Oper");
        bind(Key::Clearagain, "Clearagain");
        bind(Key::Crsel, "Crsel");
        bind(Key::Exsel, "Exsel");
        bind(Key::Kp_00, "Kp_00");
        bind(Key::Kp_000, "Kp_000");
        bind(Key::Thousandsseparator, "Thousandsseparator");
        bind(Key::Decimalseparator, "Decimalseparator");
        bind(Key::Currencyunit, "Currencyunit");
        bind(Key::Currencysubunit, "Currencysubunit");
        bind(Key::Kp_Leftparen, "Kp_Leftparen");
        bind(Key::Kp_Rightparen, "Kp_Rightparen");
        bind(Key::Kp_Leftbrace, "Kp_Leftbrace");
        bind(Key::Kp_Rightbrace, "Kp_Rightbrace");
        bind(Key::Kp_Tab, "Kp_Tab");
        bind(Key::Kp_Backspace, "Kp_Backspace");
        bind(Key::Kp_A, "Kp_A");
        bind(Key::Kp_B, "Kp_B");
        bind(Key::Kp_C, "Kp_C");
        bind(Key::Kp_D, "Kp_D");
        bind(Key::Kp_E, "Kp_E");
        bind(Key::Kp_F, "Kp_F");
        bind(Key::Kp_Xor, "Kp_Xor");
        bind(Key::Kp_Power, "Kp_Power");
        bind(Key::Kp_Percent, "Kp_Percent");
        bind(Key::Kp_Less, "Kp_Less");
        bind(Key::Kp_Greater, "Kp_Greater");
        bind(Key::Kp_Ampersand, "Kp_Ampersand");
        bind(Key::Kp_Dblampersand, "Kp_Dblampersand");
        bind(Key::Kp_Verticalbar, "Kp_Verticalbar");
        bind(Key::Kp_Dblverticalbar, "Kp_Dblverticalbar");
        bind(Key::Kp_Colon, "Kp_Colon");
        bind(Key::Kp_Hash, "Kp_Hash");
        bind(Key::Kp_Space, "Kp_Space");
        bind(Key::Kp_At, "Kp_At");
        bind(Key::Kp_Exclam, "Kp_Exclam");
        bind(Key::Kp_Memstore, "Kp_Memstore");
        bind(Key::Kp_Memrecall, "Kp_Memrecall");
        bind(Key::Kp_Memclear, "Kp_Memclear");
        bind(Key::Kp_Memadd, "Kp_Memadd");
        bind(Key::Kp_Memsubtract, "Kp_Memsubtract");
        bind(Key::Kp_Memmultiply, "Kp_Memmultiply");
        bind(Key::Kp_Memdivide, "Kp_Memdivide");
        bind(Key::Kp_Plusminus, "Kp_Plusminus");
        bind(Key::Kp_Clear, "Kp_Clear");
        bind(Key::Kp_Clearentry, "Kp_Clearentry");
        bind(Key::Kp_Binary, "Kp_Binary");
        bind(Key::Kp_Octal, "Kp_Octal");
        bind(Key::Kp_Decimal, "Kp_Decimal");
        bind(Key::Kp_Hexadecimal, "Kp_Hexadecimal");
        bind(Key::Lctrl, "Lctrl");
        bind(Key::Lshift, "Lshift");
        bind(Key::Lalt, "Lalt");
        bind(Key::Lgui, "Lgui");
        bind(Key::Rctrl, "Rctrl");
        bind(Key::Rshift, "Rshift");
        bind(Key::Ralt, "Ralt");
        bind(Key::Rgui, "Rgui");
        bind(Key::Mode, "Mode");
        bind(Key::Audionext, "Audionext");
        bind(Key::Audioprev, "Audioprev");
        bind(Key::Audiostop, "Audiostop");
        bind(Key::Audioplay, "Audioplay");
        bind(Key::Audiomute, "Audiomute");
        bind(Key::Mediaselect, "Mediaselect");
        bind(Key::Www, "Www");
        bind(Key::Mail, "Mail");
        bind(Key::Calculator, "Calculator");
        bind(Key::Computer, "Computer");
        bind(Key::Ac_Search, "Ac_Search");
        bind(Key::Ac_Home, "Ac_Home");
        bind(Key::Ac_Back, "Ac_Back");
        bind(Key::Ac_Forward, "Ac_Forward");
        bind(Key::Ac_Stop, "Ac_Stop");
        bind(Key::Ac_Refresh, "Ac_Refresh");
        bind(Key::Ac_Bookmarks, "Ac_Bookmarks");
        bind(Key::Brightnessdown, "Brightnessdown");
        bind(Key::Brightnessup, "Brightnessup");
        bind(Key::Displayswitch, "Displayswitch");
        bind(Key::Kbdillumtoggle, "Kbdillumtoggle");
        bind(Key::Kbdillumdown, "Kbdillumdown");
        bind(Key::Kbdillumup, "Kbdillumup");
        bind(Key::Eject, "Eject");
        bind(Key::Sleep, "Sleep");
      }

      void bind(Key::Type key, const std::string& name) {
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
