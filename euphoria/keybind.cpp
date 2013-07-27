// Euphoria - Copyright (c) Gustav

#include "euphoria/keybind.h"

#include <cassert>
#include <string>
#include <fstream> // NOLINT for loading data
#include <stdexcept>
#include "euphoria/str.h"
#include "json/json.h"

Keybind::Keybind(const std::string& aname, const std::string& avar,
                 const Key::Type akey)
  : name(aname), var(avar), key(akey) {
}

const std::string Keybind::getLuaVar() const {
  return var;
}

const Key::Type Keybind::getKey() const {
  return key;
}

KeybindList::KeybindList(Lua* alua)
  : lua(alua) {
  assert(lua);
}

void KeybindList::load(const std::string& filename) {
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
    keys.push_back(Keybind(name, var, key));
  }

  for (auto k : keys) {
    lua->setGlobal(k.getLuaVar(), 0.0f);
  }
}

void KeybindList::onKey(Key::Type key, bool down) {
  if (key == Key::Invalid) {
    assert(0 && "Invalid key in onKey");
    return;
  }
  if (key == Key::Unbound) {
    assert(0 && "Unbound key in onKey");
    return;
  }
  for (auto k : keys) {
    if (k.getKey() == key) {
      const float state = down ? 1.0f : 0.0f;
      lua->setGlobal(k.getLuaVar(), state);
    }
  }
}
