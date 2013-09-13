  // Euphoria - Copyright (c) Gustav
// Euphoria is awesome: 幸福感是真棒

#include "euphoria/game.h"

#ifdef USE_TWEAKABLES
#include <AntTweakBar.h>
#endif

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
#include "euphoria/entity.h"
#include "euphoria/systems.h"
#include "euphoria/settings.h"
#include "euphoria/tweak.h"
#include "euphoria/oculusvr.h"

namespace {
  Game*& GameInstance() {
    static Game* instance = 0;
    return instance;
  }
}

Game::Game(const Settings& settings, bool renderoculus)
  : keep_running_(true)
  , renderoculus_(renderoculus) {
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
  TwWindowSize(settings.width(), settings.height());
#endif

  ogldebug_.reset(new OglDebug(OglDebug::IsSupported()));

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_CULL_FACE);

  if (!GLEW_VERSION_3_2) {
    throw "System not supporting opengl 3.2";
  }

  texturecache_.reset(new TextureCache());
  shadercache_.reset(new ShaderCache());
  world_.reset(new World("world.js", texturecache_.get(), shadercache_.get()
                         , settings));


  script_.reset(new Lua());
  script_->RunFile("main.lua");

  actions_.reset(new ActionMap("actions.js", script_.get()));

  keybinds_.reset(new KeybindList());
  keybinds_->Load(actions_.get(), "keys.js", settings.control_scheme());

  camera_.reset(new Camera(settings.width(), settings.height()));
  camera_->set_fov(45);
  camera_->set_near_far(0.1f, 800.0f);

  container_.reset(new SystemContainer());
  LoadSystems("systemdefs.js", CreateSystemArg(container_.get(), world_.get(),
              texturecache_.get(), shadercache_.get(), camera_.get(),
              script_.get(), settings));

  entities_.reset(new EntityList());
  entities_->AddDefs(container_.get(), "entity.js");

  LoadEntities(entities_.get(), "entities.js");

  oculusvr_.reset(new OculusVr());

  OglDebug::Verify();
  istweaking_ = false;

  RUNTWEAKCODE(tweakers_.reset(new TweakerStore()));

  assert(GameInstance() == 0);
  GameInstance() = this;
}

Game::~Game() {
  assert(this);
  assert(this == GameInstance());
  GameInstance() = 0;

  RUNTWEAKCODE(TwTerminate());
}

bool Game::keep_running() const {
  assert(this);
  return keep_running_;
}

void ModifyCamera(Camera* cam, const EyeSetup& eye) {
  const mat44 va = eye.view_adjust();
  mat44 vaa = va;
  /// @todo fix this scaling when we have scaled the example
  cml::matrix_set_translation(vaa, cml::matrix_get_translation(va) * 10);
  cam->set_projection(eye.projection());
  cam->set_view(vaa * cam->view());
}

void SubRender(World* world, const Camera& camera) {
  world->Render(camera);
  //   if (istweaking_) {
  //     RUNTWEAKCODE(TwDraw());
  //   }
}

void RenderEye(Camera camera, const EyeSetup& eye, World* world) {
  Camera cam(camera);
  ModifyCamera(&cam, eye);
  glViewport(eye.x(), eye.y(), eye.w(), eye.h());
  SubRender(world, cam);
}

void Game::Render() {
  assert(this);
  assert(oculusvr_);
  OglDebug::Verify();

  world_->debug_renderer().Update();

  glClearColor(0, 0, 0, 1);  // black
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if (renderoculus_) {
    // create left and right camera
    RenderEye(*camera_.get(), oculusvr_->LeftEye(), world_.get());
    RenderEye(*camera_.get(), oculusvr_->RightEye(), world_.get());
  } else {
    SubRender(world_.get(), *camera_.get());
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
