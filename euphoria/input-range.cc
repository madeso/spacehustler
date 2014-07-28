// Euphoria - Copyright (c) Gustav

#include "euphoria/input-range.h"

#include <string>
#include <map>

#include "euphoria/stringutils.h"

namespace {
class RangeName {
 public:
  RangeName() {
    bind(Range::Infinite, "inf");
    bind(Range::Within01, "01");
    bind(Range::WithinNegative1Positive1, "-11");
  }

  void bind(Range range, const std::string& name) {
    strtokey.insert(std::make_pair(ToLower(name), range));
    keytostr.insert(std::make_pair(range, name));
  }

  std::string fromRange(Range range) const {
    auto r = keytostr.find(range);
    if (r == keytostr.end()) {
      return "Unknown";
    } else {
      return r->second;
    }
  }

  Range fromString(const std::string& signname) const {
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

  std::map<std::string, Range> strtokey;
  std::map<Range, std::string> keytostr;
};

const RangeName& AllRanges() {
  static RangeName names;
  return names;
}
}  // namespace

std::string ToString(Range k) { return AllRanges().fromRange(k); }

Range ToRange(const std::string& keyname) {
  return AllRanges().fromString(keyname);
}
