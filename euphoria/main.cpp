// Euphoria - Copyright (c) Gustav

#include <SFML/Window.hpp>

#include <AntTweakBar.h>

// for making wxWidgets build on osx - autodetect typetraits
#include <ciso646>
#ifdef _LIBCPP_VERSION
#define HAVE_TYPE_TRAITS
#else
#define HAVE_TR1_TYPE_TRAITS
#endif

#include <string>
#include <vector>

#include <wx/wxprec.h>  // NOLINT
#ifndef WX_PRECOMP
#include <wx/wx.h>  // NOLINT
#endif

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
#include "euphoria/systems.h"

const float pi = 3.141592653589793238462643383279502884f;

void logic() {
  srand(69);

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

#ifdef USE_TWEAKABLES
  const int twintitresult = TwInit(TW_OPENGL_CORE, NULL);  // 3.2 core profile
  if (twintitresult == 0) {
    throw TwGetLastError();
  }
  TwWindowSize(800, 600);
#endif

  OglDebug ogldebug(OglDebug::IsSupported());

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  /*if(!GLEW_VERSION_3_2)
  {
      throw "System not supporting opengl 3.2";
  }*/

  TextureCache texturecache;
  ShaderCache shadercache;
  World world("world.js", &texturecache, &shadercache);

  Camera camera;
  camera.setFov(45);
  camera.setNearFar(0.1f, 800.0f);

  SystemContainer container;
  LoadSystems("systems.js", &container, &world, &texturecache, &shadercache,
              &camera);

  EntityList entities;
  entities.addDefs(&container, "entity.js");

  LoadEntities(&entities, "entities.js");

  OglDebug::Verify();

  bool tweaking = true;

  RUNTWEAKCODE(TweakerStore tweakers);

  sf::Clock clock;
  bool running = true;
  while (running) {
    OglDebug::Verify();

    glClearColor(0, 0, 0, 1);  // black
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    world.render(camera);

    if (tweaking) {
      RUNTWEAKCODE(TwDraw());
    }

    window.display();

    bool boolean = true;

    // check all the window's events that were triggered since the last
    // iteration of the loop
    sf::Event event;
    while (window.pollEvent(event)) {
#ifdef USE_TWEAKABLES
      if (tweaking) {
        const int handled = TwEventSFML(&event, SFML_VERSION_MAJOR,
                                        SFML_VERSION_MINOR);
      }
#endif

      if (event.type == sf::Event::Closed) {
        running = false;
      }
    }

    RUNTWEAKCODE(tweakers.update());

    const sf::Time elapsed = clock.restart();
    container.step(elapsed.asSeconds());
  }

  RUNTWEAKCODE(TwTerminate());

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
