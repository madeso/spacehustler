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
#include "euphoria/keybind.h"
#include "euphoria/entity.h"
#include "euphoria/systems.h"
#include "euphoria/settings.h"
#include "euphoria/tweak.h"
#include "euphoria/oculusvr.h"
#include "euphoria/fbo.h"
#include "euphoria/quad.h"
#include "euphoria/input-action.h"

namespace {
Game*& GameInstance() {
  static Game* instance = 0;
  return instance;
}
}

#define OCULUS_TRANSFORM

Game::Game(const Settings& settings, bool renderoculus)
    : width_(settings.width()),
      height_(settings.height()),
      keep_running_(true),
      tweakaction_(0),
      last_tweak_action_(false),
      lock_mouse_(true),
      istweaking_(false),
      renderoculus_(renderoculus) {
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
    throw "System not supporting opengl 3.2";
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
  camera_->set_near_far(0.1f, 800.0f);

  container_.reset(new SystemContainer());
  LoadSystems(
      "systemdefs.js",
      CreateSystemArg(container_.get(), world_.get(), texturecache_.get(),
                      shadercache_.get(), camera_.get(), script_.get(),
                      settings, &inputsystem_));

  entities_.reset(new EntityList());
  entities_->AddDefs(container_.get(), "entity.js");

  LoadEntities(entities_.get(), "entities.js", script_.get());

  oculusvr_.reset(new OculusVr());

  if (renderoculus_) {
    const int texh = static_cast<int>(ceil(height_ * oculusvr_->get_scale()));
    const int texw = static_cast<int>(ceil(width_ * oculusvr_->get_scale()));
    eyefbo_.reset(new Fbo(texw, texh, false));
#ifdef OCULUS_TRANSFORM
    eyeprogram_ = shadercache_->GetOrCreate("oculus.js", settings);
#else
    eyeprogram_ = shadercache_->GetOrCreate("nooculus.js", settings);
#endif
    eyequad_.reset(new Quad(eyeprogram_, 1.0f, 1.0f));
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

void ModifyCamera(Camera* cam, const EyeSetup& eye,
                  const quat& oculus_orientation) {
  const mat44 va = eye.view_adjust();
  mat44 vaa = va;
  /// @todo fix this scaling when we have scaled the example
  cml::matrix_set_translation(vaa, cml::matrix_get_translation(va) * 10);
  cam->set_projection(eye.projection());
  cam->set_view(vaa * cmat44(oculus_orientation) * cam->view());
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

void RenderEye(const Camera& camera, const EyeSetup& eye, World* world,
               Fbo* fbo, Program* program, Quad* quad, bool is_right,
               const OculusVr& oculus, int window_height, int window_width,
               bool istweaking) {
  assert(fbo);
  assert(program);
  assert(quad);

  {
    Camera cam(camera);
    ModifyCamera(&cam, eye, oculus.get_orientation(true));
    TextureUpdator tex(fbo);
    ClearScreen();
    if (is_right) {
      tex.SetSubRegion(0.5f, 1.0f, 0.0f, 1.0f);
    } else {
      tex.SetSubRegion(0.0f, 0.5f, 0.0f, 1.0f);
    }
    SubRender(world, cam, istweaking);
  }

  glViewport(eye.x(), eye.y(), eye.w(), eye.h());
  program->Bind();

#ifdef OCULUS_TRANSFORM
  const float w = eye.w() / static_cast<float>(window_width);
  const float h = eye.h() / static_cast<float>(window_height);
  const float x = eye.x() / static_cast<float>(window_width);
  const float y = eye.y() / static_cast<float>(window_height);
  const float as = eye.w() / static_cast<float>(eye.h());

  // MA: This is more correct but we would need higher-res texture vertically;
  // we should adopt this once we have asymmetric input texture scale.
  const float scaleFactor = 1.0f / oculus.get_scale();

  const float dix = (is_right ? -1.0f : 1.0f) * oculus.get_center_offset()[0];

  // We are using 1/4 of DistortionCenter offset value here, since it is
  // relative to [-1,1] range that gets mapped to [0, 0.5].

  const float lcx = x + (w + dix * 0.5f) * 0.5f;
  const float lcy = y + h * 0.5f;
  program->SetUniform("LensCenter", vec2(lcx, lcy));

  const float scx = x + w * 0.5f;
  const float scy = y + h * 0.5f;
  program->SetUniform("ScreenCenter", vec2(scx, scy));

  const float sx = (w / 2) * scaleFactor;
  const float sy = (h / 2) * scaleFactor * as;
  program->SetUniform("Scale", vec2(sx, sy));

  const float six = (2 / w);
  const float siy = (2 / h) / as;
  program->SetUniform("ScaleIn", vec2(six, siy));

  program->SetUniform("HmdWarpParam", oculus.get_distortion());

  mat44 texm(w, 0, 0, x, 0, h, 0, y, 0, 0, 0, 0, 0, 0, 0, 1);
  cml::transpose(texm);
  program->SetUniform("texm", texm);
#endif

  fbo->BindTexture(0);
  quad->Render();
  program->Unbind();
}

void Game::Render() {
  assert(this);
  assert(oculusvr_);
  OglDebug::Verify();

  world_->debug_renderer().Update();

  if (renderoculus_) {
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // create left and right camera
    RenderEye(*camera_.get(), oculusvr_->LeftEye(), world_.get(), eyefbo_.get(),
              eyeprogram_.get(), eyequad_.get(), false, *oculusvr_.get(),
              height_, width_, istweaking_);
    RenderEye(*camera_.get(), oculusvr_->RightEye(), world_.get(),
              eyefbo_.get(), eyeprogram_.get(), eyequad_.get(), true,
              *oculusvr_.get(), height_, width_, istweaking_);
  } else {
    ClearScreen();
    SubRender(world_.get(), *camera_.get(), istweaking_);
  }
}

input::InputSystem& Game::inputsystem() {
  assert(this);
  return inputsystem_;
}

void Game::Update(float dt) {
  assert(this);
  RUNTWEAKCODE(tweakers_->update());
  container_->Step(dt);

  const bool tweak = tweakaction_->state() > 0.5f;

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
