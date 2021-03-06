// Euphoria - Copyright (c) Gustav

/** @file
Classes for input handling.
 */

#ifndef EUPHORIA_INPUT_DUMMYACTIVEUNIT_H_
#define EUPHORIA_INPUT_DUMMYACTIVEUNIT_H_

#include "euphoria/input-activeunit.h"

namespace euphoria {

namespace input {

/** A Dummy active unit.
 */
class DummyActiveUnit : public ActiveUnit {
 public:
  /** Rumble.
   */
  void Rumble() override;

 private:
};

}  // namespace input

}  // namespace euphoria

#endif  // EUPHORIA_INPUT_DUMMYACTIVEUNIT_H_
