// Euphoria - Copyright (c) Gustav

/** @file
Contains various helper extras for the std library.
 */

#ifndef EUPHORIA_STDUTILS_H_
#define EUPHORIA_STDUTILS_H_

#include <string>
#include <vector>
#include <map>
#include <cassert>
#include <algorithm>

#include "euphoria/str.h"

/** Generate a string representation for each of the elements in a vector.
@param c the container.
@returns the string vector.
 */
template <typename C>
std::vector<std::string> Iterate(const C& c) {
  std::vector<std::string> strings;
  for (const auto& x : c) {
    strings.push_back(Str() << x);
  }
  return strings;
}

/** Generate a string representation for each of the keys in a map.
@param m the map.
@returns the string vector.
 */
template <typename K, typename V>
std::vector<std::string> Keys(const std::map<K, V>& m) {
  std::vector<std::string> strings;
  for (const auto& x : m) {
    strings.push_back(Str() << x.first);
  }
  return strings;
}

// todo: provide an indexed version of this function as well.
template <typename T>
void SwapBackAndEraseObject(T what, std::vector<T>* from) {
  assert(from);
  typedef std::vector<T> Vec;
  typename Vec::iterator result = std::find(from->begin(), from->end(), what);
  if (result == from->end()) {
    return;
  }
  typename Vec::iterator last = from->end();
  --last;  // point to a valid entry
  std::swap(*result, *last);
  from->pop_back();
}

template <typename T>
class BuildVector {
 public:
  BuildVector& operator<<(const T& t) {
    assert(this);
    vector_.push_back(t);
    return *this;
  }

  operator std::vector<T>() {
    assert(this);
    return vector_;
  }

 private:
  std::vector<T> vector_;
};

#endif  // EUPHORIA_STDUTILS_H_
