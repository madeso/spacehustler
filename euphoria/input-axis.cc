// Euphoria - Copyright (c) Gustav

#include "euphoria/input-axis.h"

#include <string>
#include <map>

#include "euphoria/stringutils.h"

namespace {
class Signname {
 public:
  Signname() {
    bind(Sign::Positive, "+");
    bind(Sign::Negative, "-");
    bind(Sign::FullRange, "+-");
  }

  void bind(Sign key, const std::string& name) {
    strtokey.insert(std::make_pair(ToLower(name), key));
    keytostr.insert(std::make_pair(key, name));
  }

  std::string fromAxis(Sign key) const {
    auto r = keytostr.find(key);
    if (r == keytostr.end()) {
      return "Unknown";
    } else {
      return r->second;
    }
  }

  Sign fromString(const std::string& signname) const {
    if (signname == "") {
      return Sign::FullRange;
    }
    auto r = strtokey.find(ToLower(signname));
    if (r == strtokey.end()) {
      return Sign::Invalid;
    } else {
      return r->second;
    }
  }

  std::map<std::string, Sign> strtokey;
  std::map<Sign, std::string> keytostr;
};

const Signname& AllSigns() {
  static Signname names;
  return names;
}
}  // namespace

Sign ToSign(const std::string& signname) {
  return AllSigns().fromString(signname);
}

namespace {
class Axisname {
 public:
  Axisname() {
    bind(Axis::Invalid, "<invalid>");
    bind(Axis::Unbound, "<unbound>");
    bind(Axis::X, "X");
    bind(Axis::Y, "Y");
  }

  void bind(Axis key, const std::string& name) {
    strtokey.insert(std::make_pair(ToLower(name), key));
    keytostr.insert(std::make_pair(key, name));
  }

  std::string fromAxis(Axis key) const {
    auto r = keytostr.find(key);
    if (r == keytostr.end()) {
      return "Unknown";
    } else {
      return r->second;
    }
  }

  Axis fromString(const std::string& keyname) const {
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

  std::map<std::string, Axis> strtokey;
  std::map<Axis, std::string> keytostr;
};

const Axisname& AllAxes() {
  static Axisname names;
  return names;
}
}  // namespace

std::string ToString(Axis k) { return AllAxes().fromAxis(k); }

Axis ToAxis(const std::string& keyname) {
  return AllAxes().fromString(keyname);
}
