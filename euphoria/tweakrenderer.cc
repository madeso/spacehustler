// Euphoria - Copyright (c) Gustav

#include "euphoria/tweakrenderer.h"

#include <cassert>

#include "euphoria-config.h"  // NOLINT this is the default way to include cmake files

#ifdef USE_TWEAKABLES
#include <AntTweakBar.h>  // NOLINT this is where to include this
#endif

#include "euphoria/fbo.h"
#include "euphoria/quad.h"
#include "euphoria/shader.h"
#include "euphoria/camera.h"
#include "euphoria/math.h"
#include "euphoria/ogldebug.h"

namespace euphoria {

namespace {
const int FBO_WIDTH = 1024;
const int FBO_HEIGHT = 1024;
const bool FBO_MIPMAP = false;

const float QUAD_WIDTH = 30;
const float QUAD_HEIGHT = 30;
const float QUAD_Z = -15;
}

TweakRenderer::TweakRenderer(ShaderCache* shadercache, const Settings& settings,
                             bool use_ovr, int window_width,
                             int window_height) {
  assert(this);
  assert(shadercache);
  if (use_ovr == false) {
    TwWindowSize(window_width, window_height);
    return;
  }

  program_ = shadercache->GetOrCreate("default.js", settings);

  fbo_.reset(new Fbo(FBO_WIDTH, FBO_HEIGHT, FBO_MIPMAP));
  quad_ = MakeQuad(program_, QUAD_WIDTH, QUAD_HEIGHT, QUAD_Z, 1.0f, 1.0f);
  TwWindowSize(fbo_->width(), fbo_->height());
}

TweakRenderer::~TweakRenderer() { assert(this); }

void TweakRenderer::PreRender() {
  assert(this);
  if (fbo_.get() == nullptr) return;
  TextureUpdator updator(fbo_.get());
  glClearColor(0, 0, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  TwDraw();
  OglDebug::Verify();
}

void TweakRenderer::Render(const Camera& cam) {
  assert(this);
  if (fbo_.get() == nullptr) {
    TwDraw();
    return;
  }

  // setup camera matrix and draw fbo on quad

  // create a copy of the camera and zero its position
  Camera camera = cam;
  auto nv = camera.view();
  cml::matrix_set_translation(nv, CreateZeroedVec3());
  camera.set_view(nv);

  program_->Bind();
  program_->SetUniform("camera", camera.view());
  program_->SetUniform("projection", camera.projection());
  program_->SetUniform("model", Mat44Identity());
  fbo_->BindTexture(0);

  glClear(GL_DEPTH_BUFFER_BIT);
  quad_->Render();
  program_->Unbind();
  OglDebug::Verify();
}

}  // namespace euphoria
