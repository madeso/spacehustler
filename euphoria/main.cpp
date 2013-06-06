// Euphoria - Copyright (c) Gustav

#include <SFML/Window.hpp>

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

const char* const kVertexShaderSource =
  "#version 150"                                                    "\n"
  ""                                                                "\n"
  "uniform mat4 projection;"                                        "\n"
  "uniform mat4 camera;"                                            "\n"
  "uniform mat4 model;"                                             "\n"
  ""                                                                "\n"
  "in vec3 vert;"                                                   "\n"
  "in vec2 vertuv;"                                                 "\n"
  "out vec2 fraguv;"                                                "\n"
  "    "                                                            "\n"
  "void main() {"                                                   "\n"
  "    fraguv = vertuv;"                                            "\n"
  "    gl_Position = projection * camera * model * vec4(vert, 1);"  "\n"
  "}"                                                               "\n";

const char* const kFragmentShaderSource =
  "#version 150"                                                    "\n"
  ""                                                                "\n"
  "uniform sampler2D tex;"                                          "\n"
  "in vec2 fraguv;"                                                 "\n"
  "out vec4 finalColor;"                                            "\n"
  ""                                                                "\n"
  "void main() {"                                                   "\n"
  "    //set every drawn pixel to white"                            "\n"
  "    finalColor = texture(tex, fraguv);"                          "\n"
  "}"                                                               "\n";

void AddObjects(World* world) {
  boost::shared_ptr<Program> program =
    Program::FromShaderList(ShaderList()
                            (Shader::FromSource(kVertexShaderSource,
                                Shader::Vertex))
                            (Shader::FromSource(kFragmentShaderSource,
                                Shader::Fragment)));

  boost::shared_ptr<CompiledMesh> cmesh(new CompiledMesh(
                                          LoadMesh("fighter1.3ds"), program));

  boost::shared_ptr<CompiledMesh> mworld(new CompiledMesh(
      LoadMesh("world.dae"), program));

  mat44 model;
  cml::matrix_rotation_euler(model, 0.0f, 0.0f, 0.0f, cml::euler_order_yxz);

  mat44 worldmat;
  cml::matrix_translation(worldmat, vec3(-55, -20, -50));

  boost::shared_ptr<Instance> wi(new Instance(mworld, worldmat));
  world->add(wi);

  boost::shared_ptr<Instance> ip(new Instance(cmesh, model));
  world->add(ip);
}

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

  OglDebug ogldebug(OglDebug::IsSupported());

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  /*if(!GLEW_VERSION_3_2)
  {
      throw "System not supporting opengl 3.2";
  }*/


  Camera camera;
  camera.setFov(45);
  camera.setNearFar(0.1f, 200.0f);

  {
    vec3 eye, target, up;
    eye.set(30, 30, 30);
    target.zero();
    up.cardinal(1);
    cml::matrix_look_at_LH(camera.view, eye, target, up);
  }

  World world;
  AddObjects(&world);

  OglDebug::Verify();

  bool running = true;
  while (running) {
    // check all the window's events that were triggered since the last
    // iteration of the loop
    sf::Event event;
    while (window.pollEvent(event)) {
      OglDebug::Verify();

      glClearColor(0, 0, 0, 1);  // black
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      world.render(camera);

      window.display();

      if (event.type == sf::Event::Closed) {
        running = false;
      }
    }
  }

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
