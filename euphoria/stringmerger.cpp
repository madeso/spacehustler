// Euphoria - Copyright (c) Gustav

#include "euphoria/stringmerger.h"

#include <string>
#include <cassert>
#include <sstream>
#include <vector>

#include "euphoria/str.h"

std::string StringMerger::generate(
  const std::vector<std::string>& strings) const {
  assert(this);

  if (strings.empty()) {
    return Str() << start << mempty << end;
  }

  std::ostringstream ss;

  ss << start;

  const auto count = strings.size();
  for (std::vector<std::string>::size_type index = 0; index < count; ++index) {
    ss << strings[index];

    if (count != index + 1) {  // if this item isn't the last one in the list
      if (count == index + 2) {
        ss << fisep;
      } else {
        ss << sep;
      }
    }
  }

  ss << end;

  return ss.str();
}

const StringMerger& StringMerger::English() {
  static const StringMerger sep = StringMerger().between(", ", " and ")
                                  .startend("", "");
  return sep;
}

const StringMerger& StringMerger::Array() {
  static const StringMerger sep = StringMerger().between(", ")
                                  .startend("[", "]");
  return sep;
}

StringMerger& StringMerger::between(const std::string& seperator,
                                    const std::string finalSeperator) {
  assert(this);
  sep = seperator;
  fisep = finalSeperator;
  return *this;
}

StringMerger& StringMerger::between(const std::string& seperator) {
  assert(this);
  sep = seperator;
  fisep = seperator;
  return *this;
}

StringMerger& StringMerger::empty(const std::string& empty) {
  assert(this);
  mempty = empty;
  return *this;
}

StringMerger& StringMerger::startend(const std::string& astart,
                                     const std::string& aend) {
  assert(this);
  start = astart;
  end = aend;
  return *this;
}
