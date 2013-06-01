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
#include "euphoria/bitmap.h"
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

boost::shared_ptr<Bitmap> LoadBitmap(const std::string& path) {
  boost::shared_ptr<Bitmap> bitmap(new Bitmap(path));
  return bitmap;
}

boost::shared_ptr<Texture> CreateTexture(boost::shared_ptr<Bitmap> bitmap) {
  boost::shared_ptr<Texture> tex(new Texture(*bitmap.get(),
                                 Texture::Type_CompressedRgb,
                                 Texture::Wrap_MirrorRepeat,
                                 Texture::Filter_Nearest));
  return tex;
}

void AddObjects(World* world) {
  Mesh data;

  //              X     Y     Z       U     V
  // bottom
  data.addPoint(-1.0f, -1.0f, -1.0f,   0.0f, 0.0f);
  data.addPoint(1.0f, -1.0f, -1.0f,   1.0f, 0.0f);
  data.addPoint(-1.0f, -1.0f, 1.0f,   0.0f, 1.0f);
  data.addPoint(1.0f, -1.0f, -1.0f,   1.0f, 0.0f);
  data.addPoint(1.0f, -1.0f, 1.0f,   1.0f, 1.0f);
  data.addPoint(-1.0f, -1.0f, 1.0f,   0.0f, 1.0f);

  // top
  data.addPoint(-1.0f, 1.0f, -1.0f,   0.0f, 0.0f);
  data.addPoint(-1.0f, 1.0f, 1.0f,   0.0f, 1.0f);
  data.addPoint(1.0f, 1.0f, -1.0f,   1.0f, 0.0f);
  data.addPoint(1.0f, 1.0f, -1.0f,   1.0f, 0.0f);
  data.addPoint(-1.0f, 1.0f, 1.0f,   0.0f, 1.0f);
  data.addPoint(1.0f, 1.0f, 1.0f,   1.0f, 1.0f);

  // front
  data.addPoint(-1.0f, -1.0f, 1.0f,   1.0f, 0.0f);
  data.addPoint(1.0f, -1.0f, 1.0f,   0.0f, 0.0f);
  data.addPoint(-1.0f, 1.0f, 1.0f,   1.0f, 1.0f);
  data.addPoint(1.0f, -1.0f, 1.0f,   0.0f, 0.0f);
  data.addPoint(1.0f, 1.0f, 1.0f,   0.0f, 1.0f);
  data.addPoint(-1.0f, 1.0f, 1.0f,   1.0f, 1.0f);

  // back
  data.addPoint(-1.0f, -1.0f, -1.0f,   0.0f, 0.0f);
  data.addPoint(-1.0f, 1.0f, -1.0f,   0.0f, 1.0f);
  data.addPoint(1.0f, -1.0f, -1.0f,   1.0f, 0.0f);
  data.addPoint(1.0f, -1.0f, -1.0f,   1.0f, 0.0f);
  data.addPoint(-1.0f, 1.0f, -1.0f,   0.0f, 1.0f);
  data.addPoint(1.0f, 1.0f, -1.0f,   1.0f, 1.0f);

  // left
  data.addPoint(-1.0f, -1.0f, 1.0f,   0.0f, 1.0f);
  data.addPoint(-1.0f, 1.0f, -1.0f,   1.0f, 0.0f);
  data.addPoint(-1.0f, -1.0f, -1.0f,   0.0f, 0.0f);
  data.addPoint(-1.0f, -1.0f, 1.0f,   0.0f, 1.0f);
  data.addPoint(-1.0f, 1.0f, 1.0f,   1.0f, 1.0f);
  data.addPoint(-1.0f, 1.0f, -1.0f,   1.0f, 0.0f);

  // right
  data.addPoint(1.0f, -1.0f, 1.0f,   1.0f, 1.0f);
  data.addPoint(1.0f, -1.0f, -1.0f,   1.0f, 0.0f);
  data.addPoint(1.0f, 1.0f, -1.0f,   0.0f, 0.0f);
  data.addPoint(1.0f, -1.0f, 1.0f,   1.0f, 1.0f);
  data.addPoint(1.0f, 1.0f, -1.0f,   0.0f, 0.0f);
  data.addPoint(1.0f, 1.0f, 1.0f,   0.0f, 1.0f);

  boost::shared_ptr<Texture> tex =  // CreateTexture(RandomBitmap(1024, 1024));
    CreateTexture(LoadBitmap("wooden-crate.jpg"));
  //  CreateTexture(ArtyBitmap(512, 512, 100, 300));

  boost::shared_ptr<Program> program =
    Program::FromShaderList(ShaderList()
                            (Shader::FromSource(kVertexShaderSource,
                                Shader::Vertex))
                            (Shader::FromSource(kFragmentShaderSource,
                                Shader::Fragment)));

  boost::shared_ptr<CompiledMesh> cmesh(new CompiledMesh(data, program, tex));

  mat44 model;
  cml::matrix_rotation_euler(model, 0.0f, 45.0f, 0.0f, cml::euler_order_yxz);

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

  sf::Window window(sf::VideoMode(800, 600), "OpenGL", sf::Style::Default,
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

  {
    vec3 eye, target, up;
    eye.set(3, 3, 3);
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
