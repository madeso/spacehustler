Euphoria
=======

Euphoria is a basic modern opengl data driven game and rendering engine. It is not complete.

Current Features
=======
* Model loading through assimp
* Instancing meshes in a world
* Global tweak system (uses anttweakbar internally)

Build tools
=======
* CMake
* C++11 compiler
* Python, for running cpplint during build (should be possible to disable)
* Doxygen, for building documentation (should be possible to disable)
* Astyle, for styling the code (should be possible to disable)
* AssImp, for importing meshes
* OpenGL library
* SFML, for launching a window and providing a input layer
* wxWidgets for displaying error messages

Planned Features
=======
* implement all the todos
* add loading of shaders from files with preprocessor
* add physics system (ode, tokamak, physx, bullet)
* add scripting support (lua)
* audio and music support
* menu support (ce gui)
* hud/2d support
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