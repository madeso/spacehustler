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
* Joystick support (via sdl)

Required Build tools & libraries
===================
* C++11 compiler
* OpenGL
* CMake http://www.cmake.org/
* AssImp http://assimp.sourceforge.net/
* SDL2 http://libsdl.org/
* Bullet http://bulletphysics.org/wordpress/

Included libraries
===================
* AntTweakBar http://anttweakbar.sourceforge.net/doc/
* Boost  http://www.boost.org/
* CML http://cmldev.net/
* GLEW http://glew.sourceforge.net/
* JsonCpp http://jsoncpp.sourceforge.net/
* LibOVR https://developer.oculusvr.com/
* Lua http://www.lua.org/
* SOIL http://www.lonesock.net/soil.html

Optional tools
=============
* Python, for running cpplint during build
* Doxygen, for building documentation
* Astyle, for styling the code (legacy)
* clang-format, for styling the code

Planned Features and stuff that needs to be done
=======
* redesign input system
* force feedback/haptics
* complete oculus rift support
* physics motor support
* lights and shaddows
  - depth map shadows?
  - depth shadow maps/volumes?
  - http://tomdalling.com/blog/modern-opengl/06-diffuse-point-lighting/
  - http://tomdalling.com/blog/modern-opengl/07-more-lighting-ambient-specular-attenuation-gamma/
  - http://www.opengl-tutorial.org/beginners-tutorials/tutorial-8-basic-shading/
  - http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-16-shadow-mapping/
* add support for sixense (razer hydra/stem system)
* add support for leapmotion
* change from json to yaml/protobuf?
* fix luadocgen
* text rendering, distance field fonts: http://code.google.com/p/freetype-gl/ http://code.google.com/p/libgdx/wiki/DistanceFieldFonts http://bitsquid.blogspot.se/2010/04/distance-field-based-rendering-of.html
* hud/2d support
* debug overlay
* menu support (ce gui?)
* audio and music support, open al/fmod?
* particle systems http://www.opengl-tutorial.org/intermediate-tutorials/billboards-particles/billboards/
* builtin font http://nothings.org/stb/font/ http://code.google.com/p/freetype-gl/
* lua debugger/console via http/mongoose https://github.com/valenok/mongoose support ldoc?  http://stevedonovan.github.io/ldoc/topics/doc.md.html
* change all names to follow google style guide http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml#General_Naming_Rules
* implement all the todos
* add loading of shaders from files with preprocessor
* godrays http://fabiensanglard.net/lightScattering/index.php
* panning materials
* normal + specular + glow map (diffuse+alpha=transperency, specular+alpha=glow, normal+alpha=height, emissive)
* material templates with optional (lua) script
* culling + world editor
* add more script functions
  - line of sight to single (random point on object)
  - line of sight to group
  - closest of group (use kd-tree?)
  - closest object with component
* replace filehandling with a vfs
* deffered rendering
* hdr/bloom
* adaptive tone mapping
* enviromental reflection
* possible support older systems
* neurosky mindwave input? - http://store.neurosky.com/products/mindwave-1


Supported platforms
=======
* Windows 7 (Visual Studio 2012)
* Fedora 17 (KDevelop 4.4.1)