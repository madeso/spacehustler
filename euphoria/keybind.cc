// Euphoria - Copyright (c) Gustav

#include "euphoria/keybind.h"

#include <cassert>
#include <string>
#include <fstream> // NOLINT for loading data
#include <stdexcept>
#include "euphoria/str.h"
#include "json/json.h"

Action::Action(Lua* lua, const std::string& name, const std::string& var)
  : lua_(lua), name_(name), var_(var), state_(42.0f) {
  assert(this);
  assert(lua);
  set_state(0.0f);
}

float Action::state() const {
  assert(this);
  return state_;
}

void Action::set_state(float value) {
  assert(this);
  state_ = value;

  if (var_.empty() == false) {
    assert(lua_);
    lua_->SetGlobal(var_, state_);
  }
}

ActionMap::ActionMap(const std::string& filename, Lua* lua) {
  assert(this);
  std::ifstream in(filename.c_str());
  if (!in.good()) {
    throw std::logic_error(Str()
                           << "Unable to load actions from " << filename);
  }
  Json::Value root;
  Json::Reader reader;
  if (false == reader.parse(in, root)) {
    throw std::logic_error(Str() << "Unable to parse " << filename << ": "
                           << reader.getFormattedErrorMessages());
  }

  for (Json::ArrayIndex i = 0; i < root.size(); ++i) {
    Json::Value va = root[i];
    const std::string name = va.get("name", "").asString();
    const std::string var = va.get("var", "").asString();
    Action a(lua, name, var);
    actions_.insert(std::make_pair(name, a));
  }
}

Action* const ActionMap::getAction(const std::string& actionname) {
  assert(this);
  const auto r = actions_.find(actionname);
  if (r == actions_.end()) {
    throw std::logic_error(Str() << "Unable to find action " << actionname);
  }
  Action* ret = &r->second;
  return ret;
}

Keybind::Keybind(Action* const action, const Key::Type key, int device)
  : action_(action), key_(key), device_(device) {
  assert(this);
  assert(action_);
}

const Key::Type Keybind::key() const {
  assert(this);
  return key_;
}

const int Keybind::device() const {
  assert(this);
  return device_;
}

void Keybind::setState(float v) {
  assert(this);
  assert(action_);
  action_->set_state(v);
}

KeybindList::KeybindList() {
  assert(this);
}

void KeybindList::Load(ActionMap* actions, const std::string& filename) {
  assert(this);
  std::ifstream in(filename.c_str());
  if (!in.good()) {
    throw std::logic_error(Str()
                           << "Unable to load definitions from " << filename);
  }
  Json::Value root;
  Json::Reader reader;
  if (false == reader.parse(in, root)) {
    throw std::logic_error(Str() << "Unable to parse " << filename << ": "
                           << reader.getFormattedErrorMessages());
  }

  for (Json::ArrayIndex i = 0; i < root.size(); ++i) {
    Json::Value va = root[i];
    const std::string actionname = va.get("action", "").asString();
    const std::string keyname = va.get("key", "").asString();
    const int device = va.get("device", 0).asInt();

    Action* const action = actions->getAction(actionname);
    const Key::Type key = Key::FromString(keyname);
    if (key == Key::Invalid) {
      throw std::logic_error(Str() << "Invalid key for " << actionname
                             << ", got: " << keyname);
    }
    keys_.push_back(Keybind(action, key, device));
  }
}

void KeybindList::OnKey(Key::Type key, bool down) {
  const float state = down ? 1.0f : 0.0f;
  OnKey(key, state);
}

void KeybindList::OnKey(Key::Type key, float state) {
  assert(this);
  if (key == Key::Invalid) {
    assert(0 && "Invalid key in onKey");
    return;
  }
  if (key == Key::Unbound) {
    assert(0 && "Unbound key in onKey");
    return;
  }
  for (auto k : keys_) {
    if (k.key() == key) {
      k.setState(state);
    }
  }
}

void SendAxis(KeybindList* list, float value, Key::Type positive,
              Key::Type negative) {
  assert(list);
  if (value >= 0.0f) {
    list->OnKey(positive, value);
    list->OnKey(negative, 0.0f);
  } else {
    list->OnKey(positive, 0.0f);
    list->OnKey(negative, -value);
  }
}

void KeybindList::OnMouse(float dx, float dy) {
  assert(this);
  const float scale = 0.1f;
  const float dxscaled = dx * scale;
  const float dyscaled = dy * scale;
  SendAxis(this, dxscaled, Key::MouseXPositive, Key::MouseXNegative);
  SendAxis(this, dyscaled, Key::MouseYPositive, Key::MouseYNegative);
}
