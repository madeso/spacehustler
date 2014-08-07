// Euphoria - Copyright (c) Gustav

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
#include "euphoria/entity.h"
#include "euphoria/systems.h"
#include "euphoria/settings.h"
#include "euphoria/tweak.h"
#include "euphoria/oculusvr.h"
#include "euphoria/fbo.h"
#include "euphoria/quad.h"
#include "euphoria/input-globaltoggle.h"
#include "euphoria/str.h"

namespace {
Game*& GameInstance() {
  static Game* instance = 0;
  return instance;
}
}

#define OCULUS_TRANSFORM

Game::Game(const Settings& settings)
    : width_(settings.width()),
      height_(settings.height()),
      keep_running_(true),
      tweakaction_(0),
      last_tweak_action_(false),
      lock_mouse_(true),
      istweaking_(false) {
  assert(this);

  // @todo move this to a press enter to play screen
  inputsystem_.SetUnitForPlayer("Player", settings.control_scheme());

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
  TwWindowSize(width_, height_);
#endif

  ogldebug_.reset(new OglDebug(OglDebug::IsSupported()));

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_CULL_FACE);

  if (!GLEW_VERSION_3_2) {
    const std::string error = Str() << "System not supporting opengl 3.2";
    throw error;
  }

  texturecache_.reset(new TextureCache());
  shadercache_.reset(new ShaderCache());
  world_.reset(
      new World("world.js", texturecache_.get(), shadercache_.get(), settings));

  script_.reset(new Lua());
  script_->RunFile("main.lua");

  tweakaction_ = inputsystem_.GetAction("enable_tweak");

  camera_.reset(new Camera(width_, height_));
  camera_->set_fov(45);
  camera_->SetNearFar(0.1f, 800.0f);

  container_.reset(new SystemContainer());
  LoadSystems(
      "systemdefs.js",
      CreateSystemArg(container_.get(), world_.get(), texturecache_.get(),
                      shadercache_.get(), camera_.get(), script_.get(),
                      settings, &inputsystem_));

  entities_.reset(new EntityList());
  entities_->AddDefs(container_.get(), "entity.js");

  LoadEntities(entities_.get(), "entities.js", script_.get());

  if (settings.oculus_vr_detection() != OculusVrDetection::NORMAL) {
    oculusvr_.reset(new OculusVr());
    const bool allow_debug_device =
        settings.oculus_vr_detection() == OculusVrDetection::OCULUS_VR;
    oculusvr_->Detect(allow_debug_device);
  }

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
  const Mat44 va = eye.view_adjust();
  Mat44 vaa = va;
  /// @todo fix this scaling when we have scaled the example
  cml::matrix_set_translation(vaa, cml::matrix_get_translation(va) * 10);
  cam->set_projection(eye.projection());
  cam->set_view(vaa * /*CreateMat44(oculus_orientation) */ cam->view());
}

void ClearScreen() {
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void SubRender(World* world, const Camera& camera, bool istweaking) {
  world->Render(camera);
  if (istweaking) {
    RUNTWEAKCODE(TwDraw());
  }
}

void Game::Render() {
  assert(this);
  assert(oculusvr_);
  OglDebug::Verify();

  world_->debug_renderer().Update();

  if (oculusvr_->IsHmdDetected()) {
    oculusvr_->Begin();
    OglDebug::Verify();
    for (int i = 0; i < oculusvr_->GetNumberOfEyes(); ++i) {
      EyeSetup& eye = oculusvr_->GetEyeIndex(i);
      Camera cam(*camera_);
      ModifyCamera(&cam, eye);
      TextureUpdator tex(eye.GetFboPtr());
      ClearScreen();
      SubRender(world_.get(), cam, istweaking_);
    }
    OglDebug::Verify();
    oculusvr_->End();
    OglDebug::Verify();
  } else {
    ClearScreen();
    SubRender(world_.get(), *camera_, istweaking_);
  }
}

input::InputSystem& Game::inputsystem() {
  assert(this);
  return inputsystem_;
}

void Game::Update(float dt) {
  assert(this);
  RUNTWEAKCODE(tweakers_->update());
  inputsystem_.Update(dt);
  container_->Step(dt);

  const bool tweak = tweakaction_->state();

  if (tweak && tweak != last_tweak_action_) {
    istweaking_ = !istweaking_;
  }

  last_tweak_action_ = tweak;
}

bool Game::lock_mouse() const { return lock_mouse_; }

bool Game::istweaking() const {
  assert(this);
  return istweaking_;
}

void Game::Quit() {
  assert(this);
  keep_running_ = false;
}

OculusVr& Game::oculus() {
  assert(this);
  assert(oculusvr_);
  return *oculusvr_;
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
