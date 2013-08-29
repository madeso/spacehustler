FILE(GLOB game_src_glob *.cc;*.h)
set(game_src ${game_src_glob} "${PROJECT_BINARY_DIR}/euphoria-config.h")

if(USE_TWEAKABLES)
	set ( src_anttweakbar
		../external/anttweakbar/src/TwBar.cpp
		../external/anttweakbar/src/TwBar.h
		../external/anttweakbar/src/TwColors.cpp
		../external/anttweakbar/src/TwColors.h
		../external/anttweakbar/src/TwEventSFML.cpp
		../external/anttweakbar/src/TwFonts.cpp
		../external/anttweakbar/src/TwFonts.h
		../external/anttweakbar/src/TwGraph.h
		../external/anttweakbar/src/TwMgr.cpp
		../external/anttweakbar/src/TwMgr.h
		../external/anttweakbar/src/TwOpenGLCore.cpp
		../external/anttweakbar/src/TwOpenGLCore.h
		../external/anttweakbar/src/TwPrecomp.h
		../external/anttweakbar/src/res/TwXCursors.h
	)
	ADD_DEFINITIONS( -DTW_STATIC )
endif(USE_TWEAKABLES)

set ( src_jsoncpp
	../external/jsoncpp/json_reader.cpp
	../external/jsoncpp/json_value.cpp
	../external/jsoncpp/json_writer.cpp
)

set ( src_glew
	../external/glew/GL/glew.c
	../external/glew/GL/glew.h
	../external/glew/GL/wglew.h
)

set ( src_soil
	../external/soil/SOIL.c
	../external/soil/SOIL.h
	../external/soil/image_DXT.c
	../external/soil/image_DXT.h
	../external/soil/image_helper.c
	../external/soil/image_helper.h
	../external/soil/stb_image_aug.c
	../external/soil/stb_image_aug.h
	../external/soil/stbi_DDS_aug.h
	../external/soil/stbi_DDS_aug_c.h
)

set ( src_lua
	../external/lua/lua/lapi.c
	../external/lua/lua/lcode.c
	../external/lua/lua/lctype.c
	../external/lua/lua/ldebug.c
	../external/lua/lua/ldo.c
	../external/lua/lua/ldump.c
	../external/lua/lua/lfunc.c
	../external/lua/lua/lgc.c
	../external/lua/lua/llex.c
	../external/lua/lua/lmem.c
	../external/lua/lua/lobject.c
	../external/lua/lua/lopcodes.c
	../external/lua/lua/lparser.c
	../external/lua/lua/lstate.c
	../external/lua/lua/lstring.c
	../external/lua/lua/ltable.c
	../external/lua/lua/ltm.c
	../external/lua/lua/lundump.c
	../external/lua/lua/lvm.c
	../external/lua/lua/lzio.c
	../external/lua/lua/lauxlib.c
	../external/lua/lua/lbaselib.c
	../external/lua/lua/lbitlib.c
	../external/lua/lua/lcorolib.c
	../external/lua/lua/ldblib.c
	../external/lua/lua/liolib.c
	../external/lua/lua/lmathlib.c
	../external/lua/lua/loslib.c
	../external/lua/lua/lstrlib.c
	../external/lua/lua/ltablib.c
	../external/lua/lua/loadlib.c
	../external/lua/lua/linit.c
)

source_group("" FILES ${game_src})
source_group(glew FILES ${src_glew})
source_group(soil FILES ${src_soil})
source_group(lua FILES ${src_lua})
source_group(jsoncpp FILES ${src_jsoncpp})
source_group(cmake FILES "${PROJECT_SOURCE_DIR}/euphoria-config-in.h")

set(all_src ${game_src} "${PROJECT_SOURCE_DIR}/euphoria-config-in.h" ${src_glew} ${src_soil} ${src_jsoncpp} ${src_lua})

if(USE_TWEAKABLES)
	source_group(anttweakbar FILES ${src_anttweakbar})
	set(all_src ${all_src} ${src_anttweakbar})
endif(USE_TWEAKABLES)

MACRO(GenerateMain LauncherName TargetName LauncherIncludes LauncherLibraries)
	include_directories(${LauncherIncludes})
	message(STATUS "Including launcher ${LauncherName} ${LauncherIncludes}")

	source_group("" FILES "main/main-${LauncherName}.cc")
	
	add_executable(${TargetName} WIN32 ${all_src} "main/main-${LauncherName}.cc")

	if(USE_ASTYLE_BUILD)
		add_custom_command(TARGET ${TargetName}
						   PRE_BUILD
						   COMMAND astyle ARGS --options=${CMAKE_SOURCE_DIR}/astyle-options.txt ${game_src}
						   WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
						   COMMENT "Running astyle" VERBATIM)
	endif(USE_ASTYLE_BUILD)

	if(USE_CPPLINT_BUILD)
		add_custom_command(TARGET ${TargetName}
						   PRE_BUILD
						   COMMAND ./cpplint.py ARGS --output=vs7 ${game_src}
						   WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
						   COMMENT "Running CppLint" VERBATIM)
	endif(USE_CPPLINT_BUILD)

	if(USE_DOXYGEN_BUILD)
		add_custom_command(TARGET ${TargetName}
						   PRE_LINK
						   COMMAND ${DOXYGEN_EXECUTABLE} ARGS ${CMAKE_CURRENT_BINARY_DIR}/../Doxyfile
						   WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
						   COMMENT "Running Doxygen" VERBATIM)
		message(STATUS "Doxygen exe is ${DOXYGEN_EXECUTABLE} with args ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile in dir 			${CMAKE_SOURCE_DIR}/euphoria")
	endif(USE_DOXYGEN_BUILD)

	if(USE_LUADOCGEN)
		add_custom_command(TARGET ${TargetName}
						   PRE_LINK
						   COMMAND ./luadocgen.py ARGS luadoc/ ${game_src}
						   WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
						   COMMENT "Running CppLint" VERBATIM)
	endif(USE_LUADOCGEN)

	message(STATUS "Linking with OpenGL ${OPENGL_LIBRARIES}")
	message(STATUS "Linking with launcher ${LauncherName} ${LauncherLibraries}")
	message(STATUS "Linking with wxWidgets ${wxWidgets_LIBRARIES}")
	message(STATUS "Linking with AssImp ${ASSIMP_LIBRARIES}")
	message(STATUS "Linking with Bullet ${BULLET_LIBRARIES}")

	target_link_libraries(${TargetName} ${OPENGL_LIBRARIES} ${LauncherLibraries} ${wxWidgets_LIBRARIES} ${ASSIMP_LIBRARIES} ${BULLET_LIBRARIES})
ENDMACRO(GenerateMain)

option(USE_MAIN_SFML "Use sfml as launch library" ON)

if(USE_MAIN_SFML)
	if(WIN32)
		#on windows, link to sfml static, to avoid crash on exit with ati cards
		set(SFML_STATIC_LIBRARIES TRUE)
		message(STATUS "using static sfml libraries")
	else(WIN32)
		#on linux, link with dynamic since static library dependencies are horrible on linux
		message(STATUS "using non-static libraries")
	endif(WIN32)

	#on windows main is needed, not on linux
	if(WIN32)
		find_package(SFML 2 COMPONENTS system window main REQUIRED)
	else(WIN32)
		find_package(SFML 2 COMPONENTS system window REQUIRED)
	endif(WIN32)
	
	GenerateMain("sfml" main-sfml "${SFML_INCLUDE_DIR}" "${SFML_LIBRARIES}")
endif(USE_MAIN_SFML)