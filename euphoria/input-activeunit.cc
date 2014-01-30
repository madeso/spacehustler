// Euphoria - Copyright (c) Gustav

#include "euphoria/input-activeunit.h"
#include <cassert>

#include "euphoria/lua.h"
#include "euphoria/input-action.h"

namespace input {

void ActiveUnit::UpdateTable(Table* table) {
  assert(this);
  assert(table);
  for (auto action : actions_) {
    assert(action);
    if (action->scriptvarname().empty() == false) {
      table->Set(action->scriptvarname(), action->state());
    }
  }
}

ActiveUnit::~ActiveUnit() { assert(this); }

ActiveUnit::ActiveUnit() { assert(this); }

void ActiveUnit::Add(std::shared_ptr<InputAction> action) {
  assert(this);
  assert(action);
  actions_.push_back(action);
}

}  // namespace input
