// Euphoria - Copyright (c) Gustav

#include "euphoria/input-system.h"
#include <cassert>
#include <fstream>  // NOLINT for loading data

#include "json/json.h"

#include "euphoria/input-director.h"
#include "euphoria/str.h"
#include "euphoria/input-player.h"
#include "euphoria/input-keyconfig.h"

namespace input {

void Load(InputSystem* sys, const std::string& filename) {
  assert(sys);
  std::ifstream in(filename.c_str());
  if (!in.good()) {
    throw std::logic_error(Str() << "Unable to load players from " << filename);
  }
  Json::Value root;
  Json::Reader reader;
  if (false == reader.parse(in, root)) {
    throw std::logic_error(Str() << "Unable to parse " << filename << ": "
                                 << reader.getFormattedErrorMessages());
  }
  for (Json::ArrayIndex i = 0; i < root.size(); ++i) {
    const auto name = root[i].asString();
    sys->AddPlayer(name);
  }
}

InputSystem::InputSystem() : input_(new InputDirector()) {
  assert(this);
  Load(&actions_, "actions.js");
  Load(&configs_, "keys.js", actions_);
  Load(this, "players.js");
}

InputSystem::~InputSystem() {
  assert(this);
  players_.clear();
}

std::shared_ptr<GlobalToggle> InputSystem::GetAction(const std::string& name) {
  assert(this);
  return actions_.GetGlobalToggle(name);
}

void InputSystem::SetUnitForPlayer(const std::string& playerName,
                                   const std::string& inputname) {
  auto res = players_.find(playerName);
  if (res == players_.end()) {
    const std::string error = Str() << "Unable to find player " << playerName;
    throw error;
  }
  std::shared_ptr<Player> player = res->second;

  auto config = configs_.Get(inputname);
  player->set_units(config->Connect(input_.get()));
}

void InputSystem::OnKeyboardKey(Key::Type key, bool down) {
  assert(this);
  input_->OnKeyboardKey(key, down);
}

void InputSystem::OnMouseAxis(Axis::Type axis, float value) {
  assert(this);
  input_->OnMouseAxis(axis, value);
}

void InputSystem::OnMouseButton(MouseButton::Type button, bool down) {
  assert(this);
  input_->OnMouseButton(button, down);
}

void InputSystem::OnJoystickPov(Axis::Type type, int joystick, float value) {
  assert(this);
  input_->OnJoystickPov(type, joystick, value);
}

void InputSystem::OnJoystickButton(int button, int joystick, bool down) {
  assert(this);
  input_->OnJoystickButton(button, joystick, down);
}

void InputSystem::OnJoystickAxis(int axis, int joystick, float value) {
  assert(this);
  input_->OnJoystickAxis(axis, joystick, value);
}

std::shared_ptr<Player> InputSystem::GetPlayer(const std::string& name) {
  assert(this);
  auto res = players_.find(name);
  if (res == players_.end()) {
    const std::string error = Str() << "Unable to find player " << name;
    throw error;
  }
  assert(res->second);
  return res->second;
}

void InputSystem::AddPlayer(const std::string& name) {
  assert(this);
  std::shared_ptr<Player> p(new Player());
  players_.insert(std::make_pair(name, p));
}

}  // namespace input
