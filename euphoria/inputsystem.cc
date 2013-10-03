  // Euphoria - Copyright (c) Gustav
// Euphoria is awesome: 幸福感是真棒

#include "euphoria/inputsystem.h"
#include <cassert>
#include <stdexcept>
#include <fstream> // NOLINT for loading data

#include "euphoria/str.h"
#include "euphoria/lua.h"

#include "json/json.h"

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

void Load(InputActionMap* map, const std::string& filename) {
  assert(map);
  std::ifstream in(filename.c_str());
  if (!in.good()) {
    throw std::logic_error(Str()
      << "Unable to input actions from " << filename);
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
    const std::string varname = d.get("var", "").asString();
    std::shared_ptr<InputAction> action( new InputAction(varname) );
    map->Add(name, action);
  }
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

void Load(KeyConfig* config, const std::string& type, const Json::Value& data);

void Load(KeyConfigs* configs, const std::string& filename) {
  assert(configs);
  std::ifstream in(filename.c_str());
  if (!in.good()) {
    throw std::logic_error(Str()
      << "Unable to load configs from " << filename);
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
    const std::string type = d.get("type", "").asString();
    std::shared_ptr<KeyConfig> config( new KeyConfig() );
    Load(config.get(), type, d["data"]);
    configs->Add(name, config);
  }
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

//////////////////////////////////////////////////////////////////////////

class DummyActiveUnit : public ActiveUnit {
public:
  void Rumble() {
  }
private:
};

//////////////////////////////////////////////////////////////////////////

class KeyboardDef : public UnitDef {
  public:
    explicit KeyboardDef(const Json::Value& data) {
    }

    std::shared_ptr<ActiveUnit> Create() {
      std::shared_ptr<ActiveUnit> unit(new DummyActiveUnit());
      return unit;
    }

  private:
};

class MouseDef : public UnitDef {
  public:
    explicit MouseDef(const Json::Value& data) {
    }

    std::shared_ptr<ActiveUnit> Create() {
      std::shared_ptr<ActiveUnit> unit(new DummyActiveUnit());
      return unit;
    }
  private:
};

class JoystickDef : public UnitDef {
  public:
    explicit JoystickDef(const Json::Value& data) {
    }

    std::shared_ptr<ActiveUnit> Create() {
      std::shared_ptr<ActiveUnit> unit(new DummyActiveUnit());
      return unit;
    }

  private:
};

//////////////////////////////////////////////////////////////////////////

void Load(KeyConfig* config, const std::string& type, const Json::Value& data) {
  assert(config);
  std::shared_ptr<UnitDef> def;

  if( type == "keyboard" ) {
    def.reset(new KeyboardDef(data));
  } else if( type == "mouse" ) {
    def.reset(new MouseDef(data));
  } else if( type == "joystick" ) {
    def.reset(new JoystickDef(data));
  } else {
    throw std::logic_error(Str()
      << "Unknown unit definition " << type);
  }

  assert(def);
  config->Add(def);
}
