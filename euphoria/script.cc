// Euphoria - Copyright (c) Gustav

#include "euphoria/script.h"

#include <string>
#include <cassert>
#include <stdexcept>
#include <vector>
#include "euphoria/str.h"
#include "euphoria/stringmerger.h"
#include "euphoria/exception.h"

extern "C" {
#include "lua/lua.h"
#include "lua/lualib.h"
#include "lua/lauxlib.h"
}

namespace internal {
ScriptArgument::~ScriptArgument() {}

int HandleLuaException(const std::string& name, lua_State* state) {
  const std::string exception = GrabExceptionInformation();
  const std::string error = Str() << "Lua C function failure inside " << name
                                  << ": " << exception;
  lua_pushstring(state, error.c_str());
  return lua_error(state);
}

LightUserDataScriptArgument::LightUserDataScriptArgument(void** adata)
    : data_(adata) {
  assert(this);
}

bool LightUserDataScriptArgument::IsValid(lua_State* state, int position) {
  assert(this);
  auto ret = lua_islightuserdata(state, position) == 1;
  return ret;
}

void LightUserDataScriptArgument::Get(lua_State* state, int position) {
  assert(this);
  *data_ = lua_touserdata(state, position);
}

std::string LightUserDataScriptArgument::ToString() const {
  assert(this);
  return "light user data";
}

FullUserDataScriptArgument::FullUserDataScriptArgument(void** adata,
                                                       const std::string& aname)
    : data_(adata), name_(aname) {
  assert(this);
  assert(data_);
}

bool FullUserDataScriptArgument::IsValid(lua_State* state, int position) {
  assert(this);
  return luaL_checkudata(state, position, name_.c_str()) != 0;
}

void FullUserDataScriptArgument::Get(lua_State* state, int position) {
  assert(this);
  assert(data_);
  *data_ = luaL_checkudata(state, position, name_.c_str());
}

std::string FullUserDataScriptArgument::ToString() const {
  assert(this);
  return name_;
}
}  // namespace internal

namespace {
class FloatScriptArgument : public internal::ScriptArgument {
 public:
  explicit FloatScriptArgument(float* adata) : data(adata) {
    assert(this);
    assert(data);
  }

  bool IsValid(lua_State* state, int position) override {
    assert(this);
    return lua_isnumber(state, position) == 1;
  }

  void Get(lua_State* state, int position) override {
    assert(this);
    assert(data);
    *data = static_cast<float>(lua_tonumber(state, position));
  }

  std::string ToString() const {
    assert(this);
    return "float";
  }

  float* data;
};

class IntScriptArgument : public internal::ScriptArgument {
 public:
  explicit IntScriptArgument(int* adata) : data(adata) {
    assert(this);
    assert(data);
  }

  bool IsValid(lua_State* state, int position) override {
    assert(this);
    return lua_isnumber(state, position) == 1;
  }

  void Get(lua_State* state, int position) override {
    assert(this);
    assert(data);
    *data = static_cast<int>(lua_tonumber(state, position));
  }

  std::string ToString() const {
    assert(this);
    return "int";
  }

  int* data;
};

class StringScriptArgument : public internal::ScriptArgument {
 public:
  explicit StringScriptArgument(std::string* adata) : data(adata) {
    assert(this);
    assert(data);
  }

  bool IsValid(lua_State* state, int position) override {
    assert(this);
    return lua_isstring(state, position) == 1;
  }

  void Get(lua_State* state, int position) override {
    assert(this);
    assert(data);
    *data = lua_tostring(state, position);
  }

  std::string ToString() const {
    assert(this);
    return "string";
  }

  std::string* data;
};
}  // namespace

ScriptOverload::ScriptOverload(ScriptParams* aparams)
    : parameters_(aparams), is_valid_(false) {
  assert(this);
  assert(parameters_);
}

ScriptOverload& ScriptOverload::operator<<(
    std::shared_ptr<internal::ScriptArgument> arg) {
  assert(this);
  arguments_.push_back(arg);
  return *this;
}

ScriptOverload& ScriptOverload::operator<<(float* f) {
  assert(this);
  std::shared_ptr<internal::ScriptArgument> ol(new FloatScriptArgument(f));
  arguments_.push_back(ol);
  return *this;
}

ScriptOverload& ScriptOverload::operator<<(int* i) {
  assert(this);
  std::shared_ptr<internal::ScriptArgument> ol(new IntScriptArgument(i));
  arguments_.push_back(ol);
  return *this;
}

ScriptOverload& ScriptOverload::operator<<(std::string* s) {
  assert(this);
  std::shared_ptr<internal::ScriptArgument> ol(new StringScriptArgument(s));
  arguments_.push_back(ol);
  return *this;
}

ScriptOverload::operator bool() {
  assert(this);
  assert(parameters_->is_validated() == false);

  bool val = Validate(parameters_->number_of_arguments(), parameters_->state());
  if (val) {
    parameters_->SetValidated();
  } else {
    std::vector<std::string> ret;
    for (auto a : arguments_) {
      ret.push_back(a->ToString());
    }
    parameters_->AddFailure(StringMerger::Array().Generate(ret));
  }
  return val;
}

bool ScriptOverload::Validate(int argcount, lua_State* state) {
  assert(this);
  if (argcount != arguments_.size()) {
    return false;
  }

  for (int n = 0; n < argcount; ++n) {
    if (false == arguments_[n]->IsValid(state, n + 1)) {
      return false;
    }
  }

  for (int n = 0; n < argcount; ++n) {
    assert(arguments_[n]->IsValid(state, n + 1));
    arguments_[n]->Get(state, n + 1);
  }
  is_valid_ = true;
  return true;
}

ScriptParams::ScriptParams(lua_State* state)
    : state_(state),
      number_of_returns_(0),
      number_of_arguments_(0),
      is_validated_(false) {
  assert(this);
  assert(state);
  /// @todo move to the initializer list for speed and the ability make the
  /// variable a const
  number_of_arguments_ = lua_gettop(state);
}

std::vector<std::string> GetArgumentList(lua_State* state, int argcount) {
  std::vector<std::string> ret;
  for (int i = 1; i <= argcount; ++i) {
    const auto type = lua_type(state, i);
    const std::string luatypename = lua_typename(state, type);
    ret.push_back(luatypename);
  }
  return ret;
}

int ScriptParams::number_of_arguments() {
  assert(this);
  return number_of_arguments_;
}

lua_State* ScriptParams::state() {
  assert(this);
  assert(state_);
  return state_;
}

void ScriptParams::Post() {
  assert(this);
  if (is_validated() == false) {
    const std::string calledwith = StringMerger::Array().Generate(
        GetArgumentList(state_, number_of_arguments_));
    const std::string alloverloads =
        StringMerger::EnglishOr().Generate(failures_);

    throw std::logic_error(
        Str() << "Unable to determine function overload, called with "
              << calledwith << " but expected one of" << alloverloads);
  }
}

bool ScriptParams::is_validated() {
  assert(this);
  return is_validated_;
}

void ScriptParams::SetValidated() {
  assert(this);
  is_validated_ = true;
}

void ScriptParams::AddFailure(const std::string& f) {
  assert(this);
  failures_.push_back(f);
}

void ScriptParams::Return(void* userdata) {
  assert(this);
  assert(state_);
  assert(userdata);
  lua_pushlightuserdata(state_, userdata);
  ++number_of_returns_;
}

void ScriptParams::Return(const std::string& s) {
  assert(this);
  assert(state_);
  lua_pushstring(state_, s.c_str());
  ++number_of_returns_;
}

void ScriptParams::Return(float f) {
  assert(this);
  assert(state_);
  lua_pushnumber(state_, f);
  ++number_of_returns_;
}

void ScriptParams::ReturnNil() {
  assert(this);
  assert(state_);
  lua_pushnil(state_);
  ++number_of_returns_;
}

lua_State* ScriptParams::ReturnFullUserData() {
  assert(this);
  assert(state_);
  ++number_of_returns_;
  return state_;
}

int ScriptParams::number_of_returns() {
  assert(this);
  return number_of_returns_;
}

ScriptRegister::ScriptRegister() { assert(this); }

void ScriptRegister::Add(const std::string& namespaceName,
                         const std::string& name, lua_CFunction func) {
  assert(this);
  functions_[namespaceName].insert(std::make_pair(name, func));
}

void ScriptRegister::RegisterAll(lua_State* state) {
  assert(this);
  for (auto ns : functions_) {
    lua_newtable(state);  /// @todo change to luaL_newlibtable / luaL_newlib
    for (auto f : ns.second) {
      lua_pushstring(state, f.first.c_str());
      lua_pushcfunction(state, f.second);
      lua_settable(state, -3);
    }
    lua_setglobal(state, ns.first.c_str());
  }
}

ScriptRegister* GetGlobalScriptRegister() {
  static ScriptRegister reg;
  return &reg;
}
