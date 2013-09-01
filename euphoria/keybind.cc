  // Euphoria - Copyright (c) Gustav
// Euphoria is awesome: 幸福感是真棒

#include "euphoria/keybind.h"

#include <cassert>
#include <string>
#include <fstream> // NOLINT for loading data
#include <stdexcept>
#include <utility>
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

Keybind::Keybind(Action* const action, const Key::Type key, int device,
                 bool invert)
  : action_(action), key_(key), device_(device), invert_(invert) {
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
  if (invert_) {
    action_->set_state(1.0f - v);
  } else {
    action_->set_state(v);
  }
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
    Json::Value item = root[i];
    const std::string name = item.get("name", "").asString();
    if (name == "default") {
      Json::Value keybind = item["keys"];
      for (Json::ArrayIndex i = 0; i < keybind.size(); ++i) {
        Json::Value va = keybind[i];
        const std::string actionname = va.get("action", "").asString();
        const std::string keyname = va.get("key", "").asString();
        const bool invert = va.get("invert", false).asBool();
        const int device = va.get("device", 0).asInt();

        Action* const action = actions->getAction(actionname);
        const Key::Type key = Key::FromString(keyname);
        if (key == Key::Invalid) {
          throw std::logic_error(Str() << "Invalid key for " << actionname
                                 << ", got: " << keyname);
        }
        keys_.push_back(Keybind(action, key, device, invert));
      }
    }
  }
}

void KeybindList::OnKey(Key::Type key, int device, bool down) {
  const float state = down ? 1.0f : 0.0f;
  OnKey(key, device, state);
}

void KeybindList::OnKey(Key::Type key, int device, float state) {
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
    if (k.key() == key && k.device() == device) {
      float state01 = state;
      if (state01 < 0.0f) {
        state01 = 0.0f;
      }
      if (state01 > 1.0f) {
        state01 = 1.0f;
      }
      k.setState(state01);
    }
  }
}

void SendAxis(KeybindList* list, float value, Key::Type positive,
              Key::Type negative, int device) {
  assert(list);
  if (value >= 0.0f) {
    list->OnKey(positive, device, value);
    list->OnKey(negative, device, 0.0f);
  } else {
    list->OnKey(positive, device, 0.0f);
    list->OnKey(negative, device, -value);
  }
}

namespace {
  class AxisPositiveNegative {
    public:
      AxisPositiveNegative() {
        bind(Axis::MouseX,
             Key::MouseXPositive,
             Key::MouseXNegative);
        bind(Axis::MouseY,
             Key::MouseYPositive,
             Key::MouseYNegative);

        bind(Axis::JoystickX,
             Key::JoystickXPositive,
             Key::JoystickXNegative);
        bind(Axis::JoystickY,
             Key::JoystickYPositive,
             Key::JoystickYNegative);
        bind(Axis::JoystickZ,
             Key::JoystickZPositive,
             Key::JoystickZNegative);
        bind(Axis::JoystickR,
             Key::JoystickRPositive,
             Key::JoystickRNegative);
        bind(Axis::JoystickU,
             Key::JoystickUPositive,
             Key::JoystickUNegative);
        bind(Axis::JoystickV,
             Key::JoystickVPositive,
             Key::JoystickVNegative);
        bind(Axis::JoystickPovX,
             Key::JoystickPovXPositive,
             Key::JoystickPovXNegative);
        bind(Axis::JoystickPovY,
             Key::JoystickPovYPositive,
             Key::JoystickPovYNegative);
      }

      void bind(Axis::Type axis, Key::Type pos, Key::Type neg) {
        binds_.insert(std::make_pair(axis, std::make_pair(pos, neg)));
      }

      std::pair<Key::Type, Key::Type> get(Axis::Type axis) {
        auto ret = binds_.find(axis);
        assert(ret != binds_.end());
        return ret->second;
      }

    private:
      std::map<Axis::Type, std::pair<Key::Type, Key::Type>> binds_;
  };

  std::pair<Key::Type, Key::Type> AxixsLookup(Axis::Type axis) {
    static AxisPositiveNegative apn;
    return apn.get(axis);
  }
}  // namespace

Key::Type ToKey(Axis::Type axis) {
  switch (axis) {
    case Axis::MouseX:
      return Key::MouseX;
    case Axis::MouseY:
      return Key::MouseY;
    case Axis::JoystickX:
      return Key::JoystickX;
    case Axis::JoystickY:
      return Key::JoystickY;
    case Axis::JoystickZ:
      return Key::JoystickZ;
    case Axis::JoystickR:
      return Key::JoystickR;
    case Axis::JoystickU:
      return Key::JoystickU;
    case Axis::JoystickV:
      return Key::JoystickV;
    case Axis::JoystickPovX:
      return Key::JoystickPovX;
    case Axis::JoystickPovY:
      return Key::JoystickPovY;
    default:
      assert(0 && "Invalid axis");
      return Key::Unbound;
  }
}

void KeybindList::OnAxis(Axis::Type axis, int device, float state) {
  assert(this);

  const auto buttons = AxixsLookup(axis);
  SendAxis(this, state, buttons.first, buttons.second, device);
  const Key::Type key = ToKey(axis);
  if (key != Key::Unbound) {
    OnKey(key, device, (state + 1.0f) / 2.0f);
  }
}
