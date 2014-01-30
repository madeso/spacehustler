// Euphoria - Copyright (c) Gustav

#include "euphoria/input-keyconfigs.h"
#include <cassert>
#include <stdexcept>
#include <fstream>  // NOLINT for loading data

#include "euphoria/str.h"
#include "euphoria/input-keyconfig.h"

#include "euphoria/input-keyboarddef.h"
#include "euphoria/input-joystickdef.h"
#include "euphoria/input-mousedef.h"

#include "json/json.h"

namespace input {

KeyConfigs::KeyConfigs() { assert(this); }

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

std::shared_ptr<UnitDef> CreateUnit(const std::string& type,
                                    const Json::Value& data,
                                    const InputActionMap& map) {
  if (type == "keyboard") {
    std::shared_ptr<UnitDef> def;
    def.reset(new KeyboardDef(data, map));
    return def;
  } else if (type == "mouse") {
    std::shared_ptr<UnitDef> def;
    def.reset(new MouseDef(data, map));
    return def;
  } else if (type == "joystick") {
    std::shared_ptr<UnitDef> def;
    def.reset(new JoystickDef(data));
    return def;
  } else {
    throw std::logic_error(Str() << "Unknown unit definition " << type);
  }
}

void Load(KeyConfig* config, const Json::Value& units,
          const InputActionMap& map) {
  assert(config);

  for (Json::ArrayIndex i = 0; i < units.size(); ++i) {
    Json::Value d = units[i];
    const std::string type = d.get("type", "").asString();
    auto def = CreateUnit(type, d["binds"], map);
    assert(def);
    config->Add(def);
  }
}

void Load(KeyConfigs* configs, const std::string& filename,
          const InputActionMap& map) {
  assert(configs);
  std::ifstream in(filename.c_str());
  if (!in.good()) {
    throw std::logic_error(Str() << "Unable to load configs from " << filename);
  }
  Json::Value root;
  Json::Reader reader;
  if (false == reader.parse(in, root)) {
    throw std::logic_error(Str() << "Unable to parse " << filename << ": "
                                 << reader.getFormattedErrorMessages());
  }
  for (Json::ArrayIndex i = 0; i < root.size(); ++i) {
    Json::Value d = root[i];
    const std::string name = d.get("name", "").asString();
    std::shared_ptr<KeyConfig> config(new KeyConfig());
    Load(config.get(), d["units"], map);
    configs->Add(name, config);
  }
}

}  // namespace input
