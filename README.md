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
* SDL2, for launching a window and providing a input layer
* bullet for physics
* joystick support

Planned Features and stuff that needs to be done
=======
* add oculus rift support
* redesign input system
* force feedback
* lights and shaddows
* change from json to yaml/protobuf?
* fix luadocgen
* text rendering, distance field fonts: http://code.google.com/p/freetype-gl/ http://code.google.com/p/libgdx/wiki/DistanceFieldFonts http://bitsquid.blogspot.se/2010/04/distance-field-based-rendering-of.html
* hud/2d support
* debug overlay
* menu support (ce gui?)
* audio and music support
* particle systems
* builtin font http://nothings.org/stb/font/ http://code.google.com/p/freetype-gl/
* add physics motor support
* add leap motion support
* lua debugger/console via http/mongoose https://github.com/valenok/mongoose support ldoc?  http://stevedonovan.github.io/ldoc/topics/doc.md.html
* change all names to follow google style guide http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml#General_Naming_Rules
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