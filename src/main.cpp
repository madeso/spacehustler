#include <SFML/Window.hpp>
#include <GL/glew.h>
#include <string>

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

void logic()
{
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 4;
	settings.majorVersion = 3;
	settings.minorVersion = 2;

	sf::Window window(sf::VideoMode(800, 600), "OpenGL", sf::Style::Default, settings);

	GLenum err = glewInit();
	if(err != GLEW_OK)
	{
		std::string msg = (char*) glewGetErrorString(err);
		throw msg;
	}

	if(!GLEW_VERSION_3_2)
	{
		throw "System not supporting opengl 3.2";
	}

	bool running = true;
	while(running)
	{
		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;
		while(window.pollEvent(event))
		{
			glClearColor(0, 0, 0, 1); // black
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
