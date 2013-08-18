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
#include "euphoria/lua.h"
#include "euphoria/keybind.h"
#include "euphoria/game.h"
#include "euphoria/exception.h"

const float pi = 3.141592653589793238462643383279502884f;

Key::Type ToKey(sf::Event::KeyEvent key) {
  switch (key.code) {
    case sf::Keyboard::A:
      return Key::A;
    case sf::Keyboard::B:
      return Key::B;
    case sf::Keyboard::C:
      return Key::C;
    case sf::Keyboard::D:
      return Key::D;
    case sf::Keyboard::E:
      return Key::E;
    case sf::Keyboard::F:
      return Key::F;
    case sf::Keyboard::G:
      return Key::G;
    case sf::Keyboard::H:
      return Key::H;
    case sf::Keyboard::I:
      return Key::I;
    case sf::Keyboard::J:
      return Key::J;
    case sf::Keyboard::K:
      return Key::K;
    case sf::Keyboard::L:
      return Key::L;
    case sf::Keyboard::M:
      return Key::M;
    case sf::Keyboard::N:
      return Key::N;
    case sf::Keyboard::O:
      return Key::O;
    case sf::Keyboard::P:
      return Key::P;
    case sf::Keyboard::Q:
      return Key::Q;
    case sf::Keyboard::R:
      return Key::R;
    case sf::Keyboard::S:
      return Key::S;
    case sf::Keyboard::T:
      return Key::T;
    case sf::Keyboard::U:
      return Key::U;
    case sf::Keyboard::V:
      return Key::V;
    case sf::Keyboard::W:
      return Key::W;
    case sf::Keyboard::X:
      return Key::X;
    case sf::Keyboard::Y:
      return Key::Y;
    case sf::Keyboard::Z:
      return Key::Z;
    case sf::Keyboard::Num0:
      return Key::Num0;
    case sf::Keyboard::Num1:
      return Key::Num1;
    case sf::Keyboard::Num2:
      return Key::Num2;
    case sf::Keyboard::Num3:
      return Key::Num3;
    case sf::Keyboard::Num4:
      return Key::Num4;
    case sf::Keyboard::Num5:
      return Key::Num5;
    case sf::Keyboard::Num6:
      return Key::Num6;
    case sf::Keyboard::Num7:
      return Key::Num7;
    case sf::Keyboard::Num8:
      return Key::Num8;
    case sf::Keyboard::Num9:
      return Key::Num9;
    case sf::Keyboard::Escape:
      return Key::Escape;
    case sf::Keyboard::LControl:
      return Key::LControl;
    case sf::Keyboard::LShift:
      return Key::LShift;
    case sf::Keyboard::LAlt:
      return Key::LAlt;
    case sf::Keyboard::LSystem:
      return Key::LSystem;
    case sf::Keyboard::RControl:
      return Key::RControl;
    case sf::Keyboard::RShift:
      return Key::RShift;
    case sf::Keyboard::RAlt:
      return Key::RAlt;
    case sf::Keyboard::RSystem:
      return Key::RSystem;
    case sf::Keyboard::Menu:
      return Key::Menu;
    case sf::Keyboard::LBracket:
      return Key::LBracket;
    case sf::Keyboard::RBracket:
      return Key::RBracket;
    case sf::Keyboard::SemiColon:
      return Key::SemiColon;
    case sf::Keyboard::Comma:
      return Key::Comma;
    case sf::Keyboard::Period:
      return Key::Period;
    case sf::Keyboard::Quote:
      return Key::Quote;
    case sf::Keyboard::Slash:
      return Key::Slash;
    case sf::Keyboard::BackSlash:
      return Key::BackSlash;
    case sf::Keyboard::Tilde:
      return Key::Tilde;
    case sf::Keyboard::Equal:
      return Key::Equal;
    case sf::Keyboard::Dash:
      return Key::Dash;
    case sf::Keyboard::Space:
      return Key::Space;
    case sf::Keyboard::Return:
      return Key::Return;
    case sf::Keyboard::BackSpace:
      return Key::BackSpace;
    case sf::Keyboard::Tab:
      return Key::Tab;
    case sf::Keyboard::PageUp:
      return Key::PageUp;
    case sf::Keyboard::PageDown:
      return Key::PageDown;
    case sf::Keyboard::End:
      return Key::End;
    case sf::Keyboard::Home:
      return Key::Home;
    case sf::Keyboard::Insert:
      return Key::Insert;
    case sf::Keyboard::Delete:
      return Key::Delete;
    case sf::Keyboard::Add:
      return Key::Add;
    case sf::Keyboard::Subtract:
      return Key::Subtract;
    case sf::Keyboard::Multiply:
      return Key::Multiply;
    case sf::Keyboard::Divide:
      return Key::Divide;
    case sf::Keyboard::Left:
      return Key::Left;
    case sf::Keyboard::Right:
      return Key::Right;
    case sf::Keyboard::Up:
      return Key::Up;
    case sf::Keyboard::Down:
      return Key::Down;
    case sf::Keyboard::Numpad0:
      return Key::Numpad0;
    case sf::Keyboard::Numpad1:
      return Key::Numpad1;
    case sf::Keyboard::Numpad2:
      return Key::Numpad2;
    case sf::Keyboard::Numpad3:
      return Key::Numpad3;
    case sf::Keyboard::Numpad4:
      return Key::Numpad4;
    case sf::Keyboard::Numpad5:
      return Key::Numpad5;
    case sf::Keyboard::Numpad6:
      return Key::Numpad6;
    case sf::Keyboard::Numpad7:
      return Key::Numpad7;
    case sf::Keyboard::Numpad8:
      return Key::Numpad8;
    case sf::Keyboard::Numpad9:
      return Key::Numpad9;
    case sf::Keyboard::F1:
      return Key::F1;
    case sf::Keyboard::F2:
      return Key::F2;
    case sf::Keyboard::F3:
      return Key::F3;
    case sf::Keyboard::F4:
      return Key::F4;
    case sf::Keyboard::F5:
      return Key::F5;
    case sf::Keyboard::F6:
      return Key::F6;
    case sf::Keyboard::F7:
      return Key::F7;
    case sf::Keyboard::F8:
      return Key::F8;
    case sf::Keyboard::F9:
      return Key::F9;
    case sf::Keyboard::F10:
      return Key::F10;
    case sf::Keyboard::F11:
      return Key::F11;
    case sf::Keyboard::F12:
      return Key::F12;
    case sf::Keyboard::F13:
      return Key::F13;
    case sf::Keyboard::F14:
      return Key::F14;
    case sf::Keyboard::F15:
      return Key::F15;
    case sf::Keyboard::Pause:
      return Key::Pause;
    default:
      return Key::Invalid;
  }
}

Key::Type ToKey(sf::Event::MouseButtonEvent mouse) {
  switch (mouse.button) {
    case sf::Mouse::Left:
      return Key::MouseLeft;
    case sf::Mouse::Right:
      return Key::MouseRight;
    case sf::Mouse::Middle:
      return Key::MouseMiddle;
    case sf::Mouse::XButton1:
      return Key::MouseXButton1;
    case sf::Mouse::XButton2:
      return Key::MouseXButton2;
    default:
      return Key::Invalid;
  }
}

void logic() {
  srand(69);

  sf::ContextSettings settings;
  settings.depthBits = 24;
  settings.stencilBits = 8;
  settings.antialiasingLevel = 4;
  settings.majorVersion = 2;
  settings.minorVersion = 1;

  int width = 800;
  int height = 600;

  sf::Window window(sf::VideoMode(width, height), "Euphoria",
                    sf::Style::Default, settings);

  sf::VideoMode desktop = sf::VideoMode::getDesktopMode();

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

  Game game;

  Lua script;
  script.RunFile("main.lua");

  KeybindList keybinds(&script);
  keybinds.Load("keys.js");

  Camera camera;
  camera.set_fov(45);
  camera.set_near_far(0.1f, 800.0f);

  SystemContainer container;
  LoadSystems("systemdefs.js", CreateSystemArg(&container, &world,
              &texturecache, &shadercache, &camera, &script));

  EntityList entities;
  entities.AddDefs(&container, "entity.js");

  LoadEntities(&entities, "entities.js");

  OglDebug::Verify();

  bool tweaking = true;

  RUNTWEAKCODE(TweakerStore tweakers);

  sf::Clock clock;
  bool hasFocus = true;
  sf::Mouse::setPosition(sf::Vector2i(width / 2, height / 2), window);
  while (game.keep_running()) {
    OglDebug::Verify();

    glClearColor(0, 0, 0, 1);  // black
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    world.Render(camera);

    if (tweaking) {
      RUNTWEAKCODE(TwDraw());
    }

    window.display();

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

      if (event.type == sf::Event::KeyPressed
          || event.type == sf::Event::KeyReleased) {
        const bool down = event.type == sf::Event::KeyPressed;
        keybinds.OnKey(ToKey(event.key), down);
      }

      if (event.type == sf::Event::MouseButtonPressed
          || event.type == sf::Event::MouseButtonReleased) {
        const bool down = event.type == sf::Event::MouseButtonPressed;
        keybinds.OnKey(ToKey(event.mouseButton), down);
      }

      if (event.type == sf::Event::GainedFocus) {
        sf::Mouse::setPosition(sf::Vector2i(width / 2, height / 2), window);
        hasFocus = true;
      }

      if (event.type == sf::Event::LostFocus) {
        hasFocus = false;
      }

      if (event.type == sf::Event::Closed) {
        game.Quit();
      }
    }

    RUNTWEAKCODE(tweakers.update());

    if (hasFocus) {
      const sf::Vector2i mp = sf::Mouse::getPosition(window);
      sf::Mouse::setPosition(sf::Vector2i(width / 2, height / 2), window);
      float dx = (mp.x / static_cast<float>(width)) - 0.5f;
      float dy = (mp.y / static_cast<float>(height)) - 0.5f;
      keybinds.OnMouse(dx, dy);
      window.setMouseCursorVisible(false);
    } else {
      window.setMouseCursorVisible(true);
    }

    const sf::Time elapsed = clock.restart();
    container.Step(elapsed.asSeconds());
  }

  RUNTWEAKCODE(TwTerminate());

  return;
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
