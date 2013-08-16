// Euphoria - Copyright (c) Gustav

#include "euphoria/lua.h"

#include <string>
#include <cassert>
#include <stdexcept>
#include "euphoria/str.h"
#include "euphoria/script.h"
#include "euphoria/scriptlib.h"

extern "C" {
#include "lua/lua.h"
#include "lua/lualib.h"
#include "lua/lauxlib.h"
}

namespace internal {
  FunctionReturn::~FunctionReturn() {
  }
}  // namespace internal

namespace {
  void ThrowIfError(lua_State* state, int errorcode) {
    assert(state);
    if (errorcode) {
      throw std::logic_error(Str() << "Lua error: " << lua_tostring(state, -1));
      // lua_pop(L, 1); ?
    }
  }
}  // namespace

Table::Table(lua_State* astate)
  : state_(astate)
  , reference_(LUA_NOREF) {
  assert(this);
  assert(state_);
  lua_createtable(state_, 0, 0);
  reference_ = luaL_ref(state_, LUA_REGISTRYINDEX);
}

Table::~Table() {
  assert(this);
  assert(state_);
  luaL_unref(state_, LUA_REGISTRYINDEX, reference_);
  reference_ = LUA_NOREF;
}

void Table::PushToState(lua_State* astate) const {
  assert(this);
  assert(state_);
  assert(state_ == astate);
  lua_rawgeti(state_, LUA_REGISTRYINDEX, reference_);
}

class IntFunctionReturn : public internal::FunctionReturn {
  public:
    explicit IntFunctionReturn(int* i) : int_(i) {
      assert(int_);
      assert(this);
    }

    void Get(lua_State* state) {
      assert(int_);
      assert(this);
      *int_ = lua_tointeger(state, -1);
    }

  private:
    int* int_;
};

class StringFunctionReturn : public internal::FunctionReturn {
  public:
    explicit StringFunctionReturn(std::string* s) : string_(s) {
      assert(string_);
      assert(this);
    }

    void Get(lua_State* state) {
      assert(string_);
      assert(this);
      *string_ = lua_tostring(state, -1);
    }

  private:
    std::string* string_;
};

FunctionCall::FunctionCall(lua_State* astate, const std::string& name)
  : state_(astate), args_(0) {
  assert(this);
  assert(state_);
  /// @todo save and push in call function so multiple calls can be made
  /// from a single function lookup
  lua_getglobal(state_, name.c_str());
}

void FunctionCall::Argument(const std::string& str) {
  assert(this);
  assert(state_);
  lua_pushstring(state_, str.c_str());
  ++args_;
}

void FunctionCall::Argument(float f) {
  assert(this);
  assert(state_);
  lua_pushnumber(state_, f);
  ++args_;
}

void FunctionCall::Argument(const Table& t) {
  assert(this);
  assert(state_);
  t.PushToState(state_);
  ++args_;
}

void FunctionCall::Argument(int i) {
  assert(this);
  assert(state_);
  lua_pushnumber(state_, i);
  ++args_;
}

void FunctionCall::Argument(void* v) {
  assert(this);
  assert(state_);
  assert(v);
  lua_pushlightuserdata(state_, v);
  ++args_;
}

void FunctionCall::Return(std::string* str) {
  assert(this);
  assert(state_);
  assert(str);
  std::shared_ptr<internal::FunctionReturn> r(new StringFunctionReturn(str));
  returns_.push_back(r);
}

void FunctionCall::Return(int* i) {
  assert(this);
  assert(state_);
  assert(i);
  std::shared_ptr<internal::FunctionReturn> r(new IntFunctionReturn(i));
  returns_.push_back(r);
}

void FunctionCall::Call() {
  assert(this);
  assert(state_);
  assert(args_ >= 0);
  ThrowIfError(state_, lua_pcall(state_, args_, returns_.size(), 0));
  for (auto a : returns_) {
    a->Get(state_);
    lua_pop(state_, 1);
  }
  args_ = 0;
  returns_.clear();
}

Lua::Lua() : state_(luaL_newstate()) {
  assert(this);
  assert(state_);

  luaL_openlibs(state_);
  scriptlib_register(state_);
  assert(GetGlobalScriptRegister());
  GetGlobalScriptRegister()->registerAll(state_);
}

Lua::~Lua() {
  assert(this);
  assert(state_);
  lua_close(state_);
}

void Lua::RunFile(const std::string& filename) {
  assert(this);
  assert(state_);
  ThrowIfError(state_, luaL_dofile(state_, filename.c_str()));
}

void Lua::RunCode(const std::string& code) {
  assert(this);
  assert(state_);
  ThrowIfError(state_, luaL_dostring(state_, code.c_str()));
}

void Lua::SetGlobal(const std::string& name, float value) {
  assert(this);
  assert(state_);
  lua_pushnumber(state_, value);
  lua_setglobal(state_, name.c_str());
}

lua_State* Lua::state() {
  assert(this);
  assert(state_);
  return state_;
}
