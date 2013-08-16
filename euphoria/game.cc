// Euphoria - Copyright (c) Gustav

#include "euphoria/game.h"
#include <cassert>
#include <string>

#include "euphoria/script.h"

namespace {
  Game*& Instance() {
    static Game* instance = 0;
    return instance;
  }
}

Game::Game()
  : keep_running_(true) {
  assert(this);
  assert(Instance() == 0);
  Instance() = this;
}

Game::~Game() {
  assert(this);
  assert(this == Instance());
  Instance() = 0;
}

bool Game::keep_running() const {
  assert(this);
  return keep_running_;
}

void Game::Quit() {
  assert(this);
  keep_running_ = false;
}

namespace scriptinggame {
  // -- Module: Game
  const std::string LUA_MODULE_NAME = "Game";

  // -- Function: Quit
  // -- Description: Quits the game.
  void Quit(ScriptParams* params) {
    if (ScriptOverload(params)) {
      assert(Instance());
      Instance()->Quit();
    }
  }
  REGISTER_SCRIPT_FUNCTION("Quit", Quit);
}  // namespace scriptinggame
