// Euphoria - Copyright (c) Gustav

#include "euphoria/input-activeunit.h"
#include <cassert>

#include "euphoria/input-action.h"

namespace euphoria {

namespace input {

ActiveUnit::~ActiveUnit() { assert(this); }

ActiveUnit::ActiveUnit() { assert(this); }

}  // namespace input
}  // namespace euphoria
