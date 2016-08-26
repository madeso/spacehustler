// Euphoria - Copyright (c) Gustav

#include "euphoria/game.h"

#ifdef USE_TWEAKABLES
#include "imgui/imgui.h"
#endif

#include <cassert>
#include <string>

#include "euphoria/script.h"
#include "euphoria/graphics/opengl.h"
#include "euphoria/graphics/shader.h"
#include "euphoria/graphics/texture.h"
#include "euphoria/graphics/ogldebug.h"
#include "euphoria/graphics/shadercache.h"
#include "euphoria/graphics/texturecache.h"
#include "euphoria/graphics/world.h"
#include "euphoria/lua.h"
#include "euphoria/entity.h"
#include "euphoria/systems.h"
#include "euphoria/settings.h"
#include "euphoria/tweak.h"
#include "euphoria/graphics/oculusvr.h"
#include "euphoria/graphics/fbo.h"
#include "euphoria/input-globaltoggle.h"
#include "euphoria/str.h"
#include "euphoria/log.h"
#include "euphoria/ui-display.h"

#ifdef USE_TWEAKABLES
#include "euphoria/graphics/tweakrenderer.h"
#endif

namespace euphoria {

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
      istweaking_(false),
      camera_(Mat44Identity(), Mat44Identity(), settings.width(),
              settings.height()) {
  assert(this);

  // @todo move this to a press enter to play screen
  inputsystem_.SetUnitForPlayer("Player", settings.control_scheme());

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

  container_.reset(new SystemContainer());
  LoadSystems("systemdefs.js",
              CreateSystemArg(container_.get(), world_.get(),
                              texturecache_.get(), shadercache_.get(), &camera_,
                              script_.get(), settings, &inputsystem_));

  entities_.reset(new EntityList());
  entities_->AddDefs(container_.get(), "entity.js");

  LoadEntities(entities_.get(), "entities.js", script_.get());

  bool ovr = false;

  hud_.reset(new ui::Display("hud.js", settings.width(), settings.height(),
                             texturecache_.get(), shadercache_.get(),
                             settings));

  if (settings.oculus_vr_detection() != OculusVrDetection::NORMAL) {
    oculusvr_.reset(new OculusVr());
    const bool allow_debug_device =
        settings.oculus_vr_detection() == OculusVrDetection::OCULUS_VR;
    ovr = oculusvr_->Detect(settings, allow_debug_device);
  }

  OglDebug::Verify();
  istweaking_ = false;

#ifdef USE_TWEAKABLES
  tweak_renderer_.reset(
      new TweakRenderer(shadercache_.get(), settings, ovr, width_, height_));
  tweak_store_.reset(new TweakerStore());
#endif

  assert(GameInstance() == 0);
  GameInstance() = this;
}

Game::~Game() {
  assert(this);
  assert(this == GameInstance());
  GameInstance() = 0;

  Status("Closing tweakcode");
  RUNTWEAKCODE(tweak_renderer_.reset());
  RUNTWEAKCODE(tweak_store_.reset());
  Status("Terminating tweak system");
  RUNTWEAKCODE(ImGui::Shutdown());
  Status("Tweak system closed");
}

bool Game::keep_running() const {
  assert(this);
  return keep_running_;
}

void ModifyCamera(Camera* cam, const EyeSetup& eye) {
  const Mat44 view_adjust = eye.view_adjust();
  // Mat44 vaa = va;
  /// @todo fix this scaling when we have scaled the example
  // auto tran = cml::matrix_get_translation(view_adjust);
  // cml::matrix_set_translation(vaa, cml::matrix_get_translation(va) * 1);
  cam->set_projection(eye.projection());
  // cam->set_view(vaa * /*CreateMat44(oculus_orientation) */ cam->view());
  cam->set_view(view_adjust * cam->view());
}

void ClearScreen() {
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Game::SubRender(const Camera& camera) {
  world_->Render(camera);
  hud_->Render();
  if (istweaking_) {
    RUNTWEAKCODE(tweak_renderer_->Render(camera));
  }
}

void Game::Render() {
  assert(this);
  assert(oculusvr_);
  OglDebug::Verify();
  glFrontFace(GL_CCW);

  world_->debug_renderer().Update();

  RUNTWEAKCODE(tweak_renderer_->PreRender());

  if (oculusvr_->IsHmdDetected()) {
    oculusvr_->Begin();
    OglDebug::Verify();
    for (int i = 0; i < oculusvr_->GetNumberOfEyes(); ++i) {
      EyeSetup& eye = oculusvr_->GetEyeIndex(i);
      Camera cam = camera_;
      ModifyCamera(&cam, eye);
      TextureUpdator tex(eye.GetFboPtr());
      ClearScreen();
      SubRender(cam);
    }
    OglDebug::Verify();
    oculusvr_->End();
    OglDebug::Verify();
  } else {
    ClearScreen();
    SubRender(camera_);
  }
}

void Game::Update(float dt) {
  assert(this);
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

bool DismissOculusWarning(OculusVr* oculusvr, bool down) {
  assert(oculusvr);
  if (oculusvr->IsHmdDetected() && oculusvr->IsDisplayingWarning()) {
    if (down == false) {
      oculusvr->DismissWarning();
    }
  }
  return true;
}

void Game::InputOnKeyboardKey(Key key, bool down) {
  assert(this);
  if (DismissOculusWarning(oculusvr_.get(), down)) {
    inputsystem_.OnKeyboardKey(key, down);
  }
}

void Game::InputOnMouseAxis(Axis axis, float value) {
  assert(this);
  inputsystem_.OnMouseAxis(axis, value);
}

void Game::InputOnMouseButton(MouseButton button, bool down) {
  assert(this);
  if (DismissOculusWarning(oculusvr_.get(), down)) {
    inputsystem_.OnMouseButton(button, down);
  }
}

void Game::InputOnJoystickPov(Axis type, int hat, int joystick, float value) {
  assert(this);
  inputsystem_.OnJoystickPov(type, hat, joystick, value);
}

void Game::InputOnJoystickButton(int button, int joystick, bool down) {
  assert(this);
  if (DismissOculusWarning(oculusvr_.get(), down)) {
    inputsystem_.OnJoystickButton(button, joystick, down);
  }
}

void Game::InputOnJoystickAxis(int axis, int joystick, float value) {
  assert(this);
  inputsystem_.OnJoystickAxis(axis, joystick, value);
}

OculusVr& Game::oculusvr() {
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
}  // namespace euphoria
