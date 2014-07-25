// Euphoria - Copyright (c) Gustav

#include "euphoria/input-player.h"
#include <cassert>

#include "euphoria/lua.h"

namespace input {

Player::Player() { assert(this); }

void Player::UpdateTable(Table* table) {
  assert(this);
  assert(table);
  units_->UpdateTable(table);
}

void Player::Update(float dt) {
  assert(this);
  units_->Update(dt);
}

void Player::set_units(std::shared_ptr<ConnectedUnits> units) {
  assert(this);
  assert(units);
  assert(units->IsEmpty() == false);
  units_ = units;
}

}  // namespace input
