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
* padding
* mutiple tables per display
* curved ui
* hud<->game script access
* text rendering, distance field fonts: http://code.google.com/p/freetype-gl/ http://code.google.com/p/libgdx/wiki/DistanceFieldFonts http://bitsquid.blogspot.se/2010/04/distance-field-based-rendering-of.html http://stackoverflow.com/questions/11864365/good-tutorial-on-pango https://github.com/lxnt/ex-sdl-freetype-harfbuzz/ http://www.wildfiregames.com/forum/index.php?showtopic=17365 http://www.reddit.com/r/gamedev/comments/2e8q5o/2d_game_c_gui_framework_with_manually/cjyyjv6 http://www.gamedev.net/topic/609842-freetypeglvboshaders/
* fps counter
* complete oculus rift support, use DwmEnableComposition to turn off Aero https://gist.github.com/dghost/8ea97e3d4f55590d75e0 https://developer.oculusvr.com/forums/viewtopic.php?f=34&t=13682&p=182396#p182396
* disable oculus hsw in debug builds? https://developer.oculusvr.com/forums/viewtopic.php?f=20&t=11057&start=160#p182737
* transition from uniforms to uniform interface blocks in glsl: http://www.opengl.org/wiki/Interface_Block_(GLSL) http://www.lighthouse3d.com/tutorials/glsl-core-tutorial/3490-2/
* debug overlay
* 3d rendering to hud
* physics motor support
* add support for sixense (razer hydra/stem system)
* add support for leapmotion https://developer.oculusvr.com/forums/viewtopic.php?f=25&t=13471
* refactor input system with system wide documentation and war thunder input support http://forum.warthunder.com/index.php?/topic/10815-axis-and-sensitivity-settings-for-controllers-explained/ also notify http://www.gamedev.net/page/resources/_/technical/game-programming/designing-a-robust-input-handling-system-for-games-r2975
* change all names to follow google style guide http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml#General_Naming_Rules
* state system
* auto detect input system
* sdl gamepad into input system
* force feedback/haptics
* improve camera system http://www.gamedev.net/topic/427197-third-person-camera-system/#entry3842719 http://www.gamasutra.com/view/feature/132456/realtime_cameras__navigation_and_.php?print=1 http://www.gamasutra.com/blogs/EricUndersander/20131001/201382/Accurate_Collision_Zoom_for_Cameras.php http://www.gamedev.net/page/resources/_/technical/game-programming/a-simple-third-person-camera-r1591
* replace filehandling with physfs
* menu system with localization support, polyglot support? http://www.reddit.com/r/gamedev/comments/20ketu/introducing_polyglot_an_open_google_doc_with http://msdn.microsoft.com/en-us/library/dd318136%28VS.85%29.aspx http://en.cppreference.com/w/cpp/locale/locale/locale http://dukope.tumblr.com/post/83177288060/localizing-papers-please-papers-please-was
* menu with dynamic reloading of all settings
* FEEDBACK!

* write python script to also run all the postbuilds outside of the IDE http://www.cmake.org/pipermail/cmake/2010-February/034965.html
* static_assert
- rewrite to use/support fixed point arithmetic but let rendering be float/doubles - http://gameprogrammer.com/4-fixed.html http://trac.bookofhook.com/bookofhook/trac.cgi/wiki/IntroductionToFixedPointMath http://en.wikipedia.org/wiki/Libfixmath https://github.com/PetteriAimonen/libfixmatrix
* change cml to glm - glm lacks documentation & has has design flaws inherited from glsl but works better with intellisense & seems more popular https://www.opengl.org/discussion_boards/showthread.php/180191-glm-length(glm-vec)-and-glm-vec-length
* fix osx build
* upgrade assimp
* integrate viewback http://www.gamedev.net/blog/796/entry-2259935-viewback-a-video-game-design-tool/
* port to android? (physfs: http://forums.libsdl.org/viewtopic.php?p=37206&sid=3374c819e18df779e17b4ce5a49fdd15)
* check how transperency is handled https://twitter.com/ID_AA_Carmack/status/469139026708938753 http://www.openglsuperbible.com/2013/08/20/is-order-independent-transparency-really-necessary/ http://gamedev.stackexchange.com/questions/43635/what-is-the-order-less-rendering-technique-that-allows-partial-transparency
* culling + world editor with gameplay analytics http://www.gamedev.net/blog/832/entry-2259290-the-poor-mans-gameplay-analytics/
* add loading of shaders from files with preprocessor, shader editor? http://www.acegikmo.com/shaderforge/ https://code.google.com/p/wxworkspaceview/ http://sourceforge.net/projects/wxsf/ https://code.google.com/p/junkyardcode/source/browse/#svn/trunk/connex
* lights and shaddows
  - depth map shadows?
  - depth shadow maps/volumes?
  - http://tomdalling.com/blog/modern-opengl/06-diffuse-point-lighting/
  - http://tomdalling.com/blog/modern-opengl/07-more-lighting-ambient-specular-attenuation-gamma/
  - http://www.opengl-tutorial.org/beginners-tutorials/tutorial-8-basic-shading/
  - http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-16-shadow-mapping/
* change from json to yaml/protobuf?
* push new code/files + lua debugger/console via http/ighttpd http://www.lighttpd.net/ support ldoc?  http://stevedonovan.github.io/ldoc/topics/doc.md.html boost asio  http://stackoverflow.com/questions/738273/open-source-c-c-embedded-web-server or https://github.com/bel2125/civetweb http://stackoverflow.com/questions/8866809/simple-http-server-lib?lq=1 http://www.gamedev.net/blog/355/entry-2259495-why-you-should-embed-a-web-server-in-your-game-engine/
* lighttable integration?
* fix luadocgen
* Investigate constexpr
* audio and music support, fmod studio
* node scripting, https://www.youtube.com/watch?v=RsoeERfS7Gs
* sprite + line drawer in 3d in preparation for psystems
* particle systems http://www.opengl-tutorial.org/intermediate-tutorials/billboards-particles/billboards/ http://www.gamedev.net/page/resources/_/technical/graphics-programming-and-theory/flexible-particle-system-the-container-r3701 http://www.bfilipek.com/2014/06/flexible-particle-system-updaters.html http://projectgiants.tumblr.com/post/97138992417/particle-tricks-in-project-giants
* dynamically replace models/complex physics setup/damage model
* builtin font http://nothings.org/stb/font/ http://code.google.com/p/freetype-gl/
* implement all the todos
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
* refactor tweak system
* neurosky mindwave input? - http://store.neurosky.com/products/mindwave-1


Supported platforms
=======
* Windows 7 (Visual Studio 2012)
* Fedora 17 (KDevelop 4.4.1)