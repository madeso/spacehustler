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

Planned Features
=======
* lua debugger/console via http/mongoose https://github.com/valenok/mongoose support ldoc?  http://stevedonovan.github.io/ldoc/topics/doc.md.html
* audio and music support
* change from json to yaml/protobuf?
* menu support (ce gui)
* hud/2d support
* implement all the todos
* add loading of shaders from files with preprocessor
* lights and shaddows
* particle systems
* godrays
* panning materials
* normal + specular + glow map (diffuse+alpha=transperency, specular+alpha=glow, normal+alpha=height, emissive)
* material templates with optional (lua) script
* culling + world editor
* replace filehandling with a vfs
* deffered rendering
* hdr
* possible support older systems

Supported platforms
=======
* Windows 7 (Visual Studio 2012)
* Fedora 17 (KDevelop 4.4.1)