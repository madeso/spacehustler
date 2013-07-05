// Euphoria - Copyright (c) Gustav

#include "euphoria/stringseperator.h"

#include <string>
#include <cassert>
#include <sstream>

#include "euphoria/str.h"

std::string StringSeperator::toString() const {
  assert(this);

  if (strings.empty()) {
    return Str() << start << mempty << end;
  }

  std::ostringstream ss;

  ss << start;

  const Vec::size_type count = strings.size();
  for (Vec::size_type index = 0; index < count; ++index) {
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

StringSeperator& StringSeperator::english() {
  assert(this);
  return between(", ", " and ")
         .startend("", "");
}

StringSeperator& StringSeperator::array() {
  assert(this);
  return between(", ")
         .startend("[", "]");
}

StringSeperator& StringSeperator::between(const std::string& seperator,
    const std::string finalSeperator) {
  assert(this);
  sep = seperator;
  fisep = finalSeperator;
  return *this;
}

StringSeperator& StringSeperator::between(const std::string& seperator) {
  assert(this);
  sep = seperator;
  fisep = seperator;
  return *this;
}

StringSeperator& StringSeperator::empty(const std::string& empty) {
  assert(this);
  mempty = empty;
  return *this;
}

StringSeperator& StringSeperator::startend(const std::string& astart,
    const std::string& aend) {
  assert(this);
  start = astart;
  end = aend;
  return *this;
}
