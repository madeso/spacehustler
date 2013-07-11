// Euphoria - Copyright (c) Gustav

#include <SFML/Window.hpp>

#include <AntTweakBar.h>

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <string>
#include <vector>

#include "euphoria/opengl.h"

#include "euphoria/shader.h"
#include "euphoria/mesh.h"
#include "euphoria/texture.h"
#include "euphoria/color.h"
#include "euphoria/rng.h"
#include "euphoria/ogldebug.h"
#include "euphoria/math.h"
#include "euphoria/camera.h"
#include "euphoria/world.h"
#include "euphoria/tweak.h"
#include "euphoria/entity.h"

const float pi = 3.141592653589793238462643383279502884f;

boost::shared_ptr<Instance> AddObjects(TextureCache* texturecache,
                                       World* world) {
  boost::shared_ptr<CompiledMesh> cmesh(new CompiledMesh(
                                          LoadMesh("fighter1.3ds")
                                          , texturecache));

  boost::shared_ptr<CompiledMesh> mworld(new CompiledMesh(
      LoadMesh("world.dae"), texturecache));

  mat44 model;
  cml::matrix_rotation_euler(model, 0.0f, pi / 4, 0.0f
                             , cml::euler_order_yxz);

  mat44 worldmat = cmat44(vec3(-55, -20, -50));

  boost::shared_ptr<Instance> wi(new Instance(mworld, worldmat));
  world->add(wi);

  boost::shared_ptr<Instance> ip(new Instance(cmesh, model));
  world->add(ip);

  return ip;
}

void logic() {
  srand(69);

  Load(&SystemType(), "Systems.js");

  sf::ContextSettings settings;
  settings.depthBits = 24;
  settings.stencilBits = 8;
  settings.antialiasingLevel = 4;
  settings.majorVersion = 2;
  settings.minorVersion = 1;

  sf::Window window(sf::VideoMode(800, 600), "Euphoria", sf::Style::Default,
                    settings);

  const GLenum err = glewInit();
  if (err != GLEW_OK) {
    std::string msg = reinterpret_cast<const char*>(glewGetErrorString(err));
    throw msg;
  }

  const int twintitresult = TwInit(TW_OPENGL_CORE, NULL);  // 3.2 core profile
  if (twintitresult == 0) {
    throw TwGetLastError();
  }

  TwWindowSize(800, 600);

  OglDebug ogldebug(OglDebug::IsSupported());

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  /*if(!GLEW_VERSION_3_2)
  {
      throw "System not supporting opengl 3.2";
  }*/

  TextureCache texturecache;

  Camera camera;
  camera.setFov(45);
  camera.setNearFar(0.1f, 200.0f);

  {
    const vec3 eye(30, 30, 30);
    const vec3 target = cvec3zero();
    vec3 up;
    up.cardinal(1);
    cml::matrix_look_at_LH(camera.view, eye, target, up);
  }

  World world;
  auto model = AddObjects(&texturecache, &world);

  OglDebug::Verify();

  bool tweaking = true;

  TweakerStore tweakers;

  bool running = true;
  int test = 5;
  unsigned int donk = 5;
  vec3 pos = cvec3zero();
  vec3 gravity(0, 1, 0);
  quat q;
  float dog = 2.0f;
  cml::quaternion_rotation_matrix(q, model->transform);

  while (running) {
    OglDebug::Verify();

    glClearColor(0, 0, 0, 1);  // black
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    world.render(camera);

    if (tweaking) {
      TwDraw();
    }

    window.display();

    bool boolean = true;
    TWEAK(boolean);
    TWEAK(dog).minmax(0, 100).step(0.1f);
    if (boolean) {
      std::string temp = "lol";
      TWEAK(temp).group("Funny");
    }
    TWEAK(test).minmax(0, 10).step(2).hexa(true);
    TWEAK(donk).label("Integer");
    TWEAK(q);
    TWEAK(pos);
    TWEAK(gravity).isDirection(true);

    model->transform = cmat44(pos, q);

    // check all the window's events that were triggered since the last
    // iteration of the loop
    sf::Event event;
    while (window.pollEvent(event)) {
      if (tweaking) {
        const int handled = TwEventSFML(&event, SFML_VERSION_MAJOR,
                                        SFML_VERSION_MINOR);
      }

      if (event.type == sf::Event::Closed) {
        running = false;
      }
    }

    tweakers.update();
  }

  TwTerminate();

  return;
}

std::string GrabExceptionInformation() {
  try {
    throw;
  } catch (const std::exception& e) {
    return e.what();
  } catch (const char* const err) {
    return err;
  } catch (const std::string& err) {
    return err;
  } catch (...) {
    return "unknown error";
  }
  return "missing exception";
}

class MyApp: public wxApp {
    virtual bool OnInit() {
      try {
        logic();
        return false;
      } catch (...) {
        const std::string message = GrabExceptionInformation();
        const wxString error = wxString::FromAscii(message.c_str());
        wxMessageBox(error, _("Error!"), wxOK | wxICON_ERROR);
        return false;
      }
    }
};
IMPLEMENT_APP(MyApp)
