Euphoria
=======

Euphoria is a basic modern opengl data driven game and rendering engine. It is not complete.

Current Features
=======
* Model loading through assimp
* Instancing meshes in a world
* Global tweak system (uses anttweakbar internally, can be disabled)
* Physics system (bullet)
* Scripting support (lua)

Build tools
=======
* CMake
* C++11 compiler
* Python, for running cpplint during build (can be disabled)
* Doxygen, for building documentation (can be disabled)
* Astyle, for styling the code (can be disabled)
* AssImp, for importing meshes
* OpenGL library
* SFML, for launching a window and providing a input layer
* wxWidgets for displaying error messages (should be possible to disable)
* bullet for physics
* joystick support

Planned Features and stuff that needs to be done
=======
* add oculus rift support
* update anttweakbar to support oculus rift rendering & console input
* force feedback and 360 controller support
* lights and shaddows
* audio and music support
* particle systems
* fix luadocgen
* hud/2d support
* add physics motor support
* add leap motion support
* lua debugger/console via http/mongoose https://github.com/valenok/mongoose support ldoc?  http://stevedonovan.github.io/ldoc/topics/doc.md.html
* Move from SFML to SDL for forcebeedback and joystick names (optionally)
* change all names to follow google style guide http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml#General_Naming_Rules
* change from json to yaml/protobuf?
* menu support (ce gui?)
* look into joystick support and improve
* implement all the todos
* add loading of shaders from files with preprocessor
* godrays
* panning materials
* normal + specular + glow map (diffuse+alpha=transperency, specular+alpha=glow, normal+alpha=height, emissive)
* material templates with optional (lua) script
* culling + world editor
* replace filehandling with a vfs
* deffered rendering
* hdr/bloom
* adaptive tone mapping
* enviromental reflection
* possible support older systems

Supported platforms
=======
* Windows 7 (Visual Studio 2012)
* Fedora 17 (KDevelop 4.4.1)