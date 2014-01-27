// Euphoria - Copyright (c) Gustav

#include "euphoria/inputsystem.h"
#include <cassert>
#include <stdexcept>
#include <fstream>  // NOLINT for loading data

#include "euphoria/str.h"
#include "euphoria/lua.h"
#include "euphoria/key.h"

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

InputActionMap::InputActionMap() { assert(this); }

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
    throw std::logic_error(Str() << "Unable to load input actions from "
                                 << filename);
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
    std::shared_ptr<InputAction> action(new InputAction(varname));
    map->Add(name, action);
  }
}

//////////////////////////////////////////////////////////////////////////

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

void Load(KeyConfig* config, const Json::Value& data,
          const InputActionMap& map);

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

//////////////////////////////////////////////////////////////////////////

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

std::shared_ptr<InputAction> InputSystem::GetAction(const std::string& name) {
  assert(this);
  return actions_.Get(name);
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

//////////////////////////////////////////////////////////////////////////

ConnectedUnits::ConnectedUnits() { assert(this); }

void ConnectedUnits::Add(std::shared_ptr<ActiveUnit> unit) {
  assert(this);
  assert(unit);
  units_.push_back(unit);
}

void ConnectedUnits::UpdateTable(Table* table) {
  assert(this);
  assert(table);

  if (units_.empty()) {
    throw "No units connected for table update to be completed";
  }

  for (auto unit : units_) {
    assert(unit);
    unit->UpdateTable(table);
  }
}

bool ConnectedUnits::IsEmpty() const {
  assert(this);
  return units_.empty();
}

//////////////////////////////////////////////////////////////////////////

Player::Player() { assert(this); }

void Player::UpdateTable(Table* table) {
  assert(this);
  assert(table);
  units_.UpdateTable(table);
}

void Player::set_units(ConnectedUnits units) {
  assert(this);
  assert(units.IsEmpty() == false);
  units_ = units;
}

//////////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////////

KeyConfig::KeyConfig() { assert(this); }

void KeyConfig::Add(std::shared_ptr<UnitDef> def) {
  assert(this);
  assert(def);
  definitions_.push_back(def);
}

ConnectedUnits KeyConfig::Connect(InputDirector* director) const {
  ConnectedUnits units;
  for (auto def : definitions_) {
    auto unit = def->Create(director);
    assert(unit);
    units.Add(unit);
  }
  return units;
}

//////////////////////////////////////////////////////////////////////////

UnitDef::~UnitDef() { assert(this); }

//////////////////////////////////////////////////////////////////////////

class DummyActiveUnit : public ActiveUnit {
 public:
  void Rumble() {}

 private:
};

//////////////////////////////////////////////////////////////////////////

template <typename Type>
class Bind {
 public:
  Bind(Type type, std::shared_ptr<InputAction> action)
      : type_(type), action_(action) {
    assert(this);
  }

  Type type() const {
    assert(this);
    return type_;
  }

  std::shared_ptr<InputAction> action() const {
    assert(this);
    return action_;
  }

 private:
  Type type_;
  std::shared_ptr<InputAction> action_;
};

template <typename Type>
class AxisBind {
 public:
  AxisBind(Type type, std::shared_ptr<InputAction> action,
           const Json::Value& data)
      : type_(type), action_(action) {
    assert(this);

    const std::string signname = data.get("sign", "").asString();
    sign_ = Sign::FromString(signname);
    if (sign_ == Sign::Invalid) {
      const std::string error = Str() << "Invalid sign " << signname;
      throw error;
    }
  }

  Type type() const {
    assert(this);
    return type_;
  }

  std::shared_ptr<InputAction> action() const {
    assert(this);
    return action_;
  }

  Sign::Type sign() {
    assert(this);
    return sign_;
  }

 private:
  Type type_;
  std::shared_ptr<InputAction> action_;
  Sign::Type sign_;
};

//////////////////////////////////////////////////////////////////////////

class KeyboardActiveUnit : public ActiveUnit {
 public:
  KeyboardActiveUnit(const std::vector<Bind<Key::Type>>& binds,
                     InputDirector* director)
      : director_(director) {
    assert(this);
    assert(director_);

    for (auto b : binds) {
      Add(b.action());
      actions_.insert(std::make_pair(b.type(), b.action()));
    }

    director_->Add(this);
  }

  void OnKey(const Key::Type& key, bool state) {
    assert(this);
    auto res = actions_.find(key);
    if (res != actions_.end()) {
      res->second->set_state(state ? 1.0f : 0.0f);
    }
  }

  ~KeyboardActiveUnit() { director_->Remove(this); }

  void Rumble() {}

 private:
  InputDirector* director_;
  std::map<Key::Type, std::shared_ptr<InputAction>> actions_;
};

class KeyboardDef : public UnitDef {
 public:
  explicit KeyboardDef(const Json::Value& data, const InputActionMap& map) {
    for (Json::ArrayIndex i = 0; i < data.size(); ++i) {
      Json::Value d = data[i];
      const std::string keyname = d.get("key", "").asString();
      const std::string actionname = d.get("action", "").asString();
      const auto action = map.Get(actionname);
      const auto key = Key::FromString(keyname);

      if (key == Key::Invalid) {
        auto error = (Str() << keyname << " is a invalid key").ToString();
        throw error;
      }

      binds_.push_back(Bind<Key::Type>(key, action));
    }
  }

  std::shared_ptr<ActiveUnit> Create(InputDirector* director) {
    std::shared_ptr<ActiveUnit> unit(new KeyboardActiveUnit(binds_, director));
    return unit;
  }

 private:
  std::vector<Bind<Key::Type>> binds_;
};

//////////////////////////////////////////////////////////////////////////

class MouseDef : public UnitDef {
 public:
  MouseDef(const Json::Value& data, const InputActionMap& map) {
    for (Json::ArrayIndex i = 0; i < data.size(); ++i) {
      Json::Value d = data[i];
      const std::string type = d.get("type", "").asString();
      const std::string actionname = d.get("action", "").asString();
      const auto action = map.Get(actionname);

      if (type == "axis") {
        const std::string axisname = d.get("axis", "").asString();
        const auto axis = Axis::FromString(axisname);
        if (axis == Axis::Invalid) {
          const std::string error = Str() << "Invalid axis " << axisname;
          throw error;
        }
        axis_.push_back(AxisBind<Axis::Type>(axis, action, d));
      } else {
        std::string error = Str() << "Unknown input type: " << type;
        throw error;
      }
    }
  }

  std::shared_ptr<ActiveUnit> Create(InputDirector* director) {
    std::shared_ptr<ActiveUnit> unit(new DummyActiveUnit());
    return unit;
  }

 private:
  std::vector<AxisBind<Axis::Type>> axis_;
};

//////////////////////////////////////////////////////////////////////////

class JoystickDef : public UnitDef {
 public:
  explicit JoystickDef(const Json::Value& data) {}

  std::shared_ptr<ActiveUnit> Create(InputDirector* director) {
    std::shared_ptr<ActiveUnit> unit(new DummyActiveUnit());
    return unit;
  }

 private:
};

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

//////////////////////////////////////////////////////////////////////////

InputDirector::InputDirector() { assert(this); }

InputDirector::~InputDirector() { assert(this); }

void InputDirector::Add(KeyboardActiveUnit* kb) {
  assert(this);
  assert(kb);
  keyboards_.push_back(kb);
}

void InputDirector::Remove(KeyboardActiveUnit* kb) {
  assert(this);
  assert(kb);
  auto res = std::find(keyboards_.begin(), keyboards_.end(), kb);
  if (res != keyboards_.end()) {
    /// @todo implement as a swap back and erase function
    keyboards_.erase(res);
  }
}

void InputDirector::OnKeyboardKey(Key::Type key, bool down) {
  assert(this);
  for (auto kb : keyboards_) {
    kb->OnKey(key, down);
  }
}

void InputDirector::OnMouseAxis(Axis::Type axis, float value) { assert(this); }

void InputDirector::OnMouseButton(MouseButton::Type key, bool down) {
  assert(this);
}

void InputDirector::OnJoystickPov(Axis::Type type, int joystick, float value) {
  assert(this);
}

void InputDirector::OnJoystickButton(int button, int joystick, bool down) {
  assert(this);
}

void InputDirector::OnJoystickAxis(int axis, int joystick, float value) {
  assert(this);
}
