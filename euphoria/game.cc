// Euphoria - Copyright (c) Gustav

#include "euphoria/game.h"

#include <AntTweakBar.h>

#include <cassert>
#include <string>

#include "euphoria/script.h"
#include "euphoria/opengl.h"
#include "euphoria/shader.h"
#include "euphoria/texture.h"
#include "euphoria/ogldebug.h"
#include "euphoria/shadercache.h"
#include "euphoria/texturecache.h"
#include "euphoria/world.h"
#include "euphoria/lua.h"
#include "euphoria/keybind.h"
#include "euphoria/tweak.h"
#include "euphoria/entity.h"
#include "euphoria/systems.h"

namespace {
  Game*& GameInstance() {
    static Game* instance = 0;
    return instance;
  }
}

Game::Game(int width, int height)
  : keep_running_(true) {
  assert(this);

  const GLenum err = glewInit();
  if (err != GLEW_OK) {
    std::string msg = reinterpret_cast<const char*>(glewGetErrorString(err));
    throw msg;
  }

#ifdef USE_TWEAKABLES
  const int twintitresult = TwInit(TW_OPENGL_CORE, NULL);  // 3.2 core profile
  if (twintitresult == 0) {
    throw TwGetLastError();
  }
  TwWindowSize(width, height);
#endif

  ogldebug_.reset(new OglDebug(OglDebug::IsSupported()));

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_CULL_FACE);

  /*if(!GLEW_VERSION_3_2)
  {
      throw "System not supporting opengl 3.2";
  }*/

  texturecache_.reset(new TextureCache());
  shadercache_.reset(new ShaderCache());
  world_.reset(new World("world.js", texturecache_.get(), shadercache_.get()));


  script_.reset(new Lua());
  script_->RunFile("main.lua");

  actions_.reset(new ActionMap("actions.js", script_.get()));

  keybinds_.reset(new KeybindList());
  keybinds_->Load(actions_.get(), "keys.js");

  camera_.reset(new Camera());
  camera_->set_fov(45);
  camera_->set_near_far(0.1f, 800.0f);

  container_.reset(new SystemContainer());
  LoadSystems("systemdefs.js", CreateSystemArg(container_.get(), world_.get(),
              texturecache_.get(), shadercache_.get(), camera_.get(),
              script_.get()));

  entities_.reset(new EntityList());
  entities_->AddDefs(container_.get(), "entity.js");

  LoadEntities(entities_.get(), "entities.js");

  OglDebug::Verify();
  istweaking_ = true;

  RUNTWEAKCODE(tweakers_.reset(new TweakerStore()));

  assert(GameInstance() == 0);
  GameInstance() = this;
}

Game::~Game() {
  assert(this);
  assert(this == GameInstance());
  GameInstance() = 0;

  TwTerminate();
}

bool Game::keep_running() const {
  assert(this);
  return keep_running_;
}

void Game::Render() {
  assert(this);
  world_->Render(*camera_.get());

  if (istweaking_) {
    RUNTWEAKCODE(TwDraw());
  }
}

void Game::OnKey(Key::Type key, int device, float state) {
  assert(this);
  keybinds_->OnKey(key, device, state);
}
void Game::OnAxis(Axis::Type axis, int device, float state) {
  assert(this);
  keybinds_->OnAxis(axis, device, state);
}

void Game::Update(float dt) {
  assert(this);
  RUNTWEAKCODE(tweakers_->update());
  container_->Step(dt);
}

bool Game::istweaking() const {
  assert(this);
  return istweaking_;
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
      assert(GameInstance());
      GameInstance()->Quit();
    }
  }
  REGISTER_SCRIPT_FUNCTION("Quit", Quit);
}  // namespace scriptinggame
