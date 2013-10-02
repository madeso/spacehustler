  // Euphoria - Copyright (c) Gustav
// Euphoria is awesome: 幸福感是真棒

#include "euphoria/inputsystem.h"
#include <cassert>

#include "euphoria/str.h"
#include "euphoria/lua.h"

InputAction::InputAction(const std::string& scriptvarname)
  : scriptvarname_(scriptvarname), state_(0.0f) {
  assert(this);
}

const std::string& InputAction::scriptvarname() const {
  assert(this);
  return scriptvarname_;
}

float InputAction::state() const {
  assert(this);
  return state_;
}

void InputAction::set_state(float state) {
  assert(this);
  state_ = state;
}

//////////////////////////////////////////////////////////////////////////

InputActionMap::InputActionMap() {
  assert(this);
}

void InputActionMap::Add(const std::string& name,
                         std::shared_ptr<InputAction> action) {
  assert(this);
  assert(action);
  actions_.insert(std::make_pair(name, action));
}

std::shared_ptr<InputAction> InputActionMap::Get(const std::string& name)
const {
  assert(this);
  auto res = actions_.find(name);
  if (res == actions_.end()) {
    const std::string error = Str() << "Unable to find action: " << name;
    throw error;
  }

  assert(res->second);
  return res->second;
}

//////////////////////////////////////////////////////////////////////////

KeyConfigs::KeyConfigs() {
  assert(this);
}

void KeyConfigs::Add(const std::string& name,
                     std::shared_ptr<KeyConfig> config) {
  assert(this);
  assert(config);
  configs_.insert(std::make_pair(name, config));
}

std::shared_ptr<KeyConfig> KeyConfigs::Get(const std::string& name) const {
  assert(this);

  auto res = configs_.find(name);
  if (res == configs_.end()) {
    const std::string error = Str() << "Unable to find config: " << name;
    throw error;
  }

  assert(res->second);
  return res->second;
}

void KeyConfigs::BeginAutoDetect() {
  assert(this);
  assert(false && "Not implemented yet");
}

void KeyConfigs::AbortAutoDetect() {
  assert(this);
  assert(false && "Not implemented yet");
}

std::shared_ptr<ConnectedUnits> KeyConfigs::GetFirstAutoDetectedConfig() const {
  assert(this);
  assert(false && "Not implemented yet");
  std::shared_ptr<ConnectedUnits> dummy;
  return dummy;
}

//////////////////////////////////////////////////////////////////////////

InputSystem::InputSystem() {
  assert(this);
}

//////////////////////////////////////////////////////////////////////////

ConnectedUnits::ConnectedUnits() {
  assert(this);
}

void ConnectedUnits::Add(std::shared_ptr<ActiveUnit> unit) {
  assert(this);
  assert(unit);
  units_.push_back(unit);
}


//////////////////////////////////////////////////////////////////////////

Player::Player()  {
  assert(this);
}

//////////////////////////////////////////////////////////////////////////

void ActiveUnit::UpdateTable(Table* table) {
  assert(this);
  assert(table);
  for (auto action : actions_) {
    assert(action);
    table->Set(action->scriptvarname(), action->state());
  }
}

ActiveUnit::~ActiveUnit() {
  assert(this);
}

ActiveUnit::ActiveUnit() {
  assert(this);
}

void ActiveUnit::Add(std::shared_ptr<InputAction> action) {
  assert(this);
  assert(action);
  actions_.push_back(action);
}


//////////////////////////////////////////////////////////////////////////


KeyConfig::KeyConfig() {
  assert(this);
}

void KeyConfig::Add(std::shared_ptr<UnitDef> def) {
  assert(this);
  assert(def);
  definitions_.push_back(def);
}

#if 0
class KeyboardDef : public UnitDef {
  public:
    std::shared_ptr<ActiveUnit> Create() {
    }

  private:
};

class MouseDef : public UnitDef {
  public:
    std::shared_ptr<ActiveUnit> Create() {
    }
  private:
};

class JoystickDef : public UnitDef {
  public:
    std::shared_ptr<ActiveUnit> Create() {
    }

  private:
};
#endif
