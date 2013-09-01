// Euphoria - Copyright (c) Gustav

#include "euphoria/axis.h"


#include <string>
#include <map>

#include "euphoria/stringutils.h"

namespace {
  class Axisname {
    public:
      Axisname() {
        bind(Axis::Invalid, "<invalid>");
        bind(Axis::Unbound, "<unbound>");
        bind(Axis::MouseX, "MouseX");
        bind(Axis::MouseY, "MouseY");
        bind(Axis::JoystickX, "JoystickX");
        bind(Axis::JoystickY, "JoystickY");
        bind(Axis::JoystickZ, "JoystickZ");
        bind(Axis::JoystickR, "JoystickR");
        bind(Axis::JoystickU, "JoystickU");
        bind(Axis::JoystickV, "JoystickV");
        bind(Axis::JoystickPovX, "JoystickPovX");
        bind(Axis::JoystickPovY, "JoystickPovY");
      }

      void bind(Axis::Type key, const std::string name) {
        strtokey.insert(std::make_pair(ToLower(name), key));
        keytostr.insert(std::make_pair(key, name));
      }

      std::string fromAxis(Axis::Type key) const {
        auto r = keytostr.find(key);
        if (r == keytostr.end()) {
          return "Unknown";
        } else {
          return r->second;
        }
      }

      Axis::Type fromString(const std::string& keyname) const {
        if (keyname == "") {
          return Axis::Unbound;
        }
        auto r = strtokey.find(ToLower(keyname));
        if (r == strtokey.end()) {
          return Axis::Invalid;
        } else {
          return r->second;
        }
      }

      std::map<std::string, Axis::Type> strtokey;
      std::map<Axis::Type, std::string> keytostr;
  };

  const Axisname& AllAxes() {
    static Axisname names;
    return names;
  }
}  // namespace

namespace Axis {
  std::string ToString(Type k) {
    return AllAxes().fromAxis(k);
  }

  Type FromString(const std::string& keyname) {
    return AllAxes().fromString(keyname);
  }
}  // namespace Axis