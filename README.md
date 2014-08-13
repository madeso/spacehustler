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
* AssImp, BSD, http://assimp.sourceforge.net/
* SDL2, zlib, http://libsdl.org/
* Bullet, zlib,  http://bulletphysics.org/wordpress/

Included libraries
===================
* AntTweakBar, zlib, http://anttweakbar.sourceforge.net/doc/
* Boost, boost license, http://www.boost.org/
* CML, boost license, http://cmldev.net/
* GLEW, bsd+mit, http://glew.sourceforge.net/
* JsonCpp, mit/public domain, http://jsoncpp.sourceforge.net/
* LibOVR, custom license(free to use, distribute + modify, but must share modifications with oculus), https://developer.oculusvr.com/
* Lua, mit, http://www.lua.org/
* SOIL, public domain, http://www.lonesock.net/soil.html

Optional tools
=============
* Python, for running cpplint during build
* Doxygen, for building documentation
* clang-format, for styling the code

Planned Features and stuff that needs to be done
=======
* move all code into a euphoria namespace
* update tweak system for oculus support
* complete oculus rift support
* hud/2d support
* text rendering, distance field fonts: http://code.google.com/p/freetype-gl/ http://code.google.com/p/libgdx/wiki/DistanceFieldFonts http://bitsquid.blogspot.se/2010/04/distance-field-based-rendering-of.html
* debug overlay
* physics motor support
* add support for sixense (razer hydra/stem system)
* add support for leapmotion
* refactor input system with system wide documentation and war thunder input support http://forum.warthunder.com/index.php?/topic/10815-axis-and-sensitivity-settings-for-controllers-explained/
* state system
* auto detect input system
* sdl gamepad into input system
* force feedback/haptics
* FEEDBACK
* write python script to also run all the postbuilds outside of the IDE http://www.cmake.org/pipermail/cmake/2010-February/034965.html
* static_assert
* change cml to glm
* fix osx build
* upgrade assimp
* port to android?
* replace filehandling with physfs (on android: http://forums.libsdl.org/viewtopic.php?p=37206&sid=3374c819e18df779e17b4ce5a49fdd15)
* check how transperency is handled https://twitter.com/ID_AA_Carmack/status/469139026708938753 http://www.openglsuperbible.com/2013/08/20/is-order-independent-transparency-really-necessary/ http://gamedev.stackexchange.com/questions/43635/what-is-the-order-less-rendering-technique-that-allows-partial-transparency
* culling + world editor
* lights and shaddows
  - depth map shadows?
  - depth shadow maps/volumes?
  - http://tomdalling.com/blog/modern-opengl/06-diffuse-point-lighting/
  - http://tomdalling.com/blog/modern-opengl/07-more-lighting-ambient-specular-attenuation-gamma/
  - http://www.opengl-tutorial.org/beginners-tutorials/tutorial-8-basic-shading/
  - http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-16-shadow-mapping/
* change from json to yaml/protobuf?
* fix luadocgen
* menu support (ce gui?)
* Investigate constexpr
* audio and music support, fmod studio
* sprite + line drawer
* particle systems http://www.opengl-tutorial.org/intermediate-tutorials/billboards-particles/billboards/ http://www.gamedev.net/page/resources/_/technical/graphics-programming-and-theory/flexible-particle-system-the-container-r3701 http://www.bfilipek.com/2014/06/flexible-particle-system-updaters.html
* dynamically replace models/complex physics setup/damage model
* builtin font http://nothings.org/stb/font/ http://code.google.com/p/freetype-gl/
* push new code/files + lua debugger/console via http/ighttpd http://www.lighttpd.net/ support ldoc?  http://stevedonovan.github.io/ldoc/topics/doc.md.html boost asio  http://stackoverflow.com/questions/738273/open-source-c-c-embedded-web-server or https://github.com/bel2125/civetweb http://stackoverflow.com/questions/8866809/simple-http-server-lib?lq=1
* change all names to follow google style guide http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml#General_Naming_Rules
* implement all the todos
* add loading of shaders from files with preprocessor
* godrays http://fabiensanglard.net/lightScattering/index.php
* panning materials
* normal + specular + glow map (diffuse+alpha=transperency, specular+alpha=glow, normal+alpha=height, emissive)
* Clouds! http://www.markmark.net/clouds/
* material templates with optional (lua) script
* add more script functions
  - line of sight to single (random point on object)
  - line of sight to group
  - closest of group (use kd-tree?)
  - closest object with component
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