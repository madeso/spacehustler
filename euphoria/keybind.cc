// Euphoria - Copyright (c) Gustav

#include "euphoria/keybind.h"

#include <cassert>
#include <string>
#include <fstream> // NOLINT for loading data
#include <stdexcept>
#include "euphoria/str.h"
#include "json/json.h"

Keybind::Keybind(const std::string& name, const std::string& var,
                 const Key::Type key)
  : name_(name), var_(var), key_(key) {
  assert(this);
}

const std::string Keybind::var() const {
  assert(this);
  return var_;
}

const Key::Type Keybind::key() const {
  assert(this);
  return key_;
}

KeybindList::KeybindList(Lua* lua)
  : lua_(lua) {
  assert(this);
  assert(lua_);
}

void KeybindList::Load(const std::string& filename) {
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
    const std::string name = va.get("name", "untitled key").asString();
    const std::string var = va.get("scriptvar", "").asString();
    const std::string keyname = va.get("key", "").asString();
    const Key::Type key = Key::FromString(keyname);
    if (key == Key::Invalid) {
      throw std::logic_error(Str() << "Invalid key for " << name
                             << ", got: " << keyname);
    }
    keys_.push_back(Keybind(name, var, key));
  }

  for (auto k : keys_) {
    lua_->SetGlobal(k.var(), 0.0f);
  }
}

void KeybindList::OnKey(Key::Type key, bool down) {
  assert(this);
  assert(lua_);
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
      const float state = down ? 1.0f : 0.0f;
      lua_->SetGlobal(k.var(), state);
    }
  }
}

void KeybindList::OnMouse(float dx, float dy) {
  assert(this);
  assert(lua_);
  lua_->SetGlobal("mousex", dx);
  lua_->SetGlobal("mousey", dy);
}
