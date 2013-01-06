#include <SFML/Window.hpp>
#include "opengl.h"
#include <string>
#include <vector>

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "shader.h"
#include "mesh.h"
#include "bitmap.h"
#include "texture.h"
#include "color.h"

const char* const kVertexShaderSource =
   "#version 150"									"\n"
   "	"											"\n"
   "in vec4 vert;"									"\n"
   "in vec2 vertuv;"								"\n"
   "out vec2 fraguv;"								"\n"
   "	"											"\n"
   "void main() {"									"\n"
   "    fraguv = vertuv;"							"\n"
   "    gl_Position = vert;"						"\n"
   "}"												"\n";

const char* const kFragmentShaderSource =
   "#version 150"									"\n"
   ""												"\n"
   "uniform sampler2D tex;"							"\n"
   "in vec2 fraguv;"								"\n"
   "out vec4 finalColor;"							"\n"
   ""												"\n"
   "void main() {"									"\n"
   "    //set every drawn pixel to white"			"\n"
   "    finalColor = texture(tex, fraguv);"			"\n"
   "}"												"\n";

float RandomFloat()
{
	return rand() / static_cast<float>(RAND_MAX);
}

Color RandomColor()
{
	return Color(RandomFloat(), RandomFloat(), RandomFloat());
}

boost::shared_ptr<Bitmap> RandomBitmap(int width, int height)
{
	boost::shared_ptr<Bitmap> bitmap( new Bitmap(width, height, Bitmap::Rgb) );
	for(int x=0; x<width; ++x)
	{
		for(int y=0; y<height; ++y)
		{
			bitmap->setPixel(x, y, RandomColor() );
		}
	}

	//bitmap->save();
	return bitmap;
}

boost::shared_ptr<Texture> RandomTexture(int width, int height)
{
	boost::shared_ptr<Bitmap> bitmap = RandomBitmap(width, height);
	boost::shared_ptr<Texture> tex(new Texture(*bitmap.get(), Texture::Type_CompressedRgb, Texture::Wrap_ClampToEdge, Texture::Filter_Nearest) );
	return tex;
}

void logic()
{
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 4;
	settings.majorVersion = 2;
	settings.minorVersion = 1;

	sf::Window window(sf::VideoMode(800, 600), "OpenGL", sf::Style::Default, settings);
	window.setVisible(false);

	const GLenum err = glewInit();
	if(err != GLEW_OK)
	{
		std::string msg = (char*) glewGetErrorString(err);
		throw msg;
	}

	/*if(!GLEW_VERSION_3_2)
	{
		throw "System not supporting opengl 3.2";
	}*/

	Mesh data;
	data.addPoint(0.0f, 0.8f, 0.0f,   0.5f, 1.0f);
	data.addPoint(-0.8f,-0.8f, 0.0f,   0.0f, 1.0f);
	data.addPoint(0.8f,-0.8f, 0.0f,   1.0f, 0.0f);

	boost::shared_ptr<Program> program = Program::FromShaderList(
	                                        ShaderList()
	                                        (Shader::FromSource(kVertexShaderSource, Shader::Vertex))
	                                        (Shader::FromSource(kFragmentShaderSource, Shader::Fragment))
	                                     );
	CompiledMesh cmesh(data, *program.get());
	boost::shared_ptr<Texture> tex = RandomTexture(1024, 1024);

	window.setVisible(true);
	bool running = true;
	while(running)
	{
		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;
		while(window.pollEvent(event))
		{
			glClearColor(0, 0, 0, 1); // black
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


			program->bind();
			tex->bind(0);
			program->setUniform("tex", 0);
			cmesh.render();
			program->unbind();

			window.display();

			if(event.type == sf::Event::Closed)
			{
				running = false;
			}
		}
	}

	return;
}

std::string GrabExceptionInformation()
{
	try
	{
		throw;
	}
	catch(const std::exception& e)
	{
		return e.what();
	}
	catch(const char* const err)
	{
		return err;
	}
	catch(const std::string& err)
	{
		return err;
	}
	catch(...)
	{
		return "unknown error";
	}
	return "missing excception";
}

class MyApp: public wxApp
{
	virtual bool OnInit()
	{
		try
		{
			logic();
			return false;
		}
		catch(...)
		{
			const std::string message = GrabExceptionInformation();
			const wxString error = wxString::FromAscii(message.c_str());
			wxMessageBox(error, _("Error!"), wxOK | wxICON_ERROR);
			return false;
		}
	}
};
IMPLEMENT_APP(MyApp)
