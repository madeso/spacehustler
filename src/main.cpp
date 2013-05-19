#include <string>
#include <vector>

#include <SFML/Window.hpp>

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "opengl.h"

#include "shader.h"
#include "mesh.h"
#include "bitmap.h"
#include "texture.h"
#include "color.h"
#include "rng.h"
#include "ogldebug.h"

const char* const kVertexShaderSource =
  "#version 150"                                   "\n"
  "    "                                           "\n"
  "in vec4 vert;"                                  "\n"
  "in vec2 vertuv;"                                "\n"
  "out vec2 fraguv;"                               "\n"
  "    "                                           "\n"
  "void main() {"                                  "\n"
  "    fraguv = vertuv;"                           "\n"
  "    gl_Position = vert;"                        "\n"
  "}"                                              "\n";

const char* const kFragmentShaderSource =
  "#version 150"                                   "\n"
  ""                                               "\n"
  "uniform sampler2D tex;"                         "\n"
  "in vec2 fraguv;"                                "\n"
  "out vec4 finalColor;"                           "\n"
  ""                                               "\n"
  "void main() {"                                  "\n"
  "    //set every drawn pixel to white"           "\n"
  "    finalColor = texture(tex, fraguv);"         "\n"
  "}"                                              "\n";

float RandomFloat() {
  static Rng r(42);
  return r.nextFloat();
}

Color RandomColor() {
  return Color(RandomFloat(), RandomFloat(), RandomFloat());
}

boost::shared_ptr<Bitmap> RandomBitmap(int width, int height) {
  boost::shared_ptr<Bitmap> bitmap(new Bitmap(width, height, Bitmap::Rgb));
  for (int x = 0; x < width; ++x) {
    for (int y = 0; y < height; ++y) {
      bitmap->setPixel(x, y, RandomColor());
    }
  }

  // bitmap->save();
  return bitmap;
}

int RandomInt(int i) {
  return static_cast<int>(RandomFloat() * i);
}

void PlaceDot(boost::shared_ptr<Bitmap> bitmap, int dx, int dy, const Color& c,
              int radius) {
  const int rr = radius * radius;
  assert(radius != 0);
  for (int x = dx - radius; x < dx + radius; ++x) {
    for (int y = dy - radius; y < dy + radius; ++y) {
      if (x < 0) {
        continue;
      }
      if (y < 0) {
        continue;
      }
      if (x >= bitmap->getWidth()) {
        continue;
      }
      if (y >= bitmap->getHeight()) {
        continue;
      }
      const int xx = dx - x;
      const int yy = dy - y;
      const int ll = xx * xx + yy * yy;
      if (ll <= rr) {
        const Color nc(c, 1 - (ll / static_cast<float>(rr)));
        bitmap->paintPixel(x, y, nc);
      }
    }
  }
}

boost::shared_ptr<Bitmap> ArtyBitmap(int width, int height, unsigned int dots,
                                     int dotsize) {
  // create white bitmap
  boost::shared_ptr<Bitmap> bitmap(new Bitmap(width, height, Bitmap::Rgb));
  for (int x = 0; x < width; ++x) {
    for (int y = 0; y < height; ++y) {
      bitmap->setPixel(x, y, Color(1.0f));
    }
  }

  for (unsigned int i = 0; i < dots; ++i) {
    PlaceDot(bitmap, RandomInt(width), RandomInt(height), RandomColor(),
             1 + RandomInt(dotsize));
  }

  // bitmap->save();
  return bitmap;
}

boost::shared_ptr<Texture> CreateTexture(boost::shared_ptr<Bitmap> bitmap) {
  boost::shared_ptr<Texture> tex(new Texture(*bitmap.get(),
                                 Texture::Type_CompressedRgb,
                                 Texture::Wrap_MirrorRepeat,
                                 Texture::Filter_Nearest));
  return tex;
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

  /*if(!GLEW_VERSION_3_2)
  {
      throw "System not supporting opengl 3.2";
  }*/

  Mesh data;
  data.addPoint(0.0f, 0.8f, 0.0f,   0.5f, 1.0f);
  data.addPoint(-0.8f, -0.8f, 0.0f,   0.0f, 0.0f);
  data.addPoint(0.8f, -0.8f, 0.0f,   1.0f, 0.0f);

  boost::shared_ptr<Program> program =
    Program::FromShaderList(ShaderList()
                            (Shader::FromSource(kVertexShaderSource,
                                Shader::Vertex))
                            (Shader::FromSource(kFragmentShaderSource,
                                Shader::Fragment)));

  CompiledMesh cmesh(data, *program.get());

  boost::shared_ptr<Texture> tex =  // CreateTexture(RandomBitmap(1024, 1024));
    CreateTexture(ArtyBitmap(512, 512, 100, 300));
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

      program->bind();
      tex->bind(0);
      program->setUniform("tex", 0);
      cmesh.render();
      program->unbind();

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
  return "missing excception";
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
