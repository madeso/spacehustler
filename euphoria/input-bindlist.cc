// Euphoria - Copyright (c) Gustav

#include "euphoria/input-bindlist.h"

#include <cassert>

#include "euphoria/input-activeaxis.h"
#include "euphoria/input-activerange.h"
#include "euphoria/input-activerangetoaxis.h"
#include "euphoria/input-action.h"

#include "euphoria/lua.h"

namespace input {

void BindList::UpdateTable(Table* table) {
  assert(this);
  assert(table);
  for (auto range : ranges_) {
    assert(range);
    if (range->action().scriptvarname().empty() == false) {
      table->Set(range->action().scriptvarname(), range->state());
    }
  }

  for (auto axis : axes_) {
    assert(axis);
    if (axis->action().scriptvarname().empty() == false) {
      table->Set(axis->action().scriptvarname(), axis->state());
    }
  }

  for (auto range : rangeToAxes_) {
    assert(range);
    if (range->action().scriptvarname().empty() == false) {
      table->Set(range->action().scriptvarname(), range->state());
    }
  }
}

}  // namespace input
