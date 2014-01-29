// Euphoria - Copyright (c) Gustav

#include "euphoria/range.h"

#include <string>
#include <map>

#include "euphoria/stringutils.h"

namespace {
class RangeName {
 public:
  RangeName() {
    bind(Range::Infinite, "inf");
    bind(Range::Within01, "01");
  }

  void bind(Range::Type range, const std::string& name) {
    strtokey.insert(std::make_pair(ToLower(name), range));
    keytostr.insert(std::make_pair(range, name));
  }

  std::string fromRange(Range::Type range) const {
    auto r = keytostr.find(range);
    if (r == keytostr.end()) {
      return "Unknown";
    } else {
      return r->second;
    }
  }

  Range::Type fromString(const std::string& signname) const {
    if (signname == "") {
      return Range::Infinite;
    }
    auto r = strtokey.find(ToLower(signname));
    if (r == strtokey.end()) {
      return Range::Invalid;
    } else {
      return r->second;
    }
  }

  std::map<std::string, Range::Type> strtokey;
  std::map<Range::Type, std::string> keytostr;
};

const RangeName& AllRanges() {
  static RangeName names;
  return names;
}
}  // namespace

namespace Range {
std::string ToString(Type k) { return AllRanges().fromRange(k); }

Type FromString(const std::string& keyname) {
  return AllRanges().fromString(keyname);
}
}  // namespace Range
