// Euphoria - Copyright (c) Gustav

#include "euphoria/input-actionmap.h"
#include <cassert>
#include <stdexcept>
#include <fstream>  // NOLINT for loading data

#include "euphoria/str.h"
#include "json/json.h"

#include "euphoria/input-range.h"
#include "euphoria/input-action.h"
#include "euphoria/input-globaltoggle.h"

namespace euphoria {

namespace input {

InputActionMap::InputActionMap() { assert(this); }

void InputActionMap::Update() {
  assert(this);
  for (auto t : toggles_) {
    (t.second)->Update();
  }
}

void InputActionMap::Add(const std::string& name,
                         std::shared_ptr<InputAction> action) {
  assert(this);
  assert(action);
  actions_.insert(std::make_pair(name, action));
  if (action->global()) {
    std::shared_ptr<GlobalToggle> toggle(new GlobalToggle(action));
    toggles_.insert(std::make_pair(name, toggle));
  }
}

std::shared_ptr<InputAction> InputActionMap::Get(
    const std::string& name) const {
  assert(this);
  auto res = actions_.find(name);
  if (res == actions_.end()) {
    const std::string error = Str() << "Unable to find action: " << name;
    throw error;
  }

  assert(res->second);
  return res->second;
}

std::shared_ptr<GlobalToggle> InputActionMap::GetGlobalToggle(
    const std::string& name) const {
  assert(this);
  auto res = toggles_.find(name);
  if (res == toggles_.end()) {
    const std::string error = Str() << "Unable to find toggle: " << name;
    throw error;
  }

  assert(res->second);
  return res->second;
}

std::vector<std::shared_ptr<InputAction>> InputActionMap::GetActionList()
    const {
  assert(this);
  std::vector<std::shared_ptr<InputAction>> ret;
  for (auto a : actions_) {
    ret.push_back(a.second);
  }
  return ret;
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
    const bool global = d.get("global", false).asBool();
    const std::string rangename = d.get("range", "").asString();
    const Range range = ToRange(rangename);
    if (range == Range::INVALID) {
      const std::string error = Str() << "Invalid range: " << rangename
                                      << " for the " << name << " action";
      throw error;
    }
    std::shared_ptr<InputAction> action(
        new InputAction(name, varname, range, global));
    map->Add(name, action);
  }
}

}  // namespace input
}  // namespace euphoria
