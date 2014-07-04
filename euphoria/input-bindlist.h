// Euphoria - Copyright (c) Gustav

/** @file
Classes for input handling.
 */

#ifndef EUPHORIA_INPUT_BINDLIST_H_
#define EUPHORIA_INPUT_BINDLIST_H_

#include <vector>

namespace input {

class Bind;

/** A list of binds.
 */
class BindList {
 public:
  // update table
 private:
  std::vector<Bind*> binds_;
};

}  // namespace input

#endif  // EUPHORIA_INPUT_BINDLIST_H_
