FILE(GLOB game_src_glob *.cpp;*.h)
set(game_src ${game_src_glob} "${PROJECT_BINARY_DIR}/euphoria-config.h")

if(USE_TWEAKABLES)
	set ( src_anttweakbar
		../external/anttweakbar/src/TwEventSFML.cpp
		../external/anttweakbar/src/TwBar.cpp
		../external/anttweakbar/src/TwColors.cpp
		../external/anttweakbar/src/TwFonts.cpp
		../external/anttweakbar/src/TwMgr.cpp
		../external/anttweakbar/src/TwOpenGLCore.cpp
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
	../external/lua/src/lapi.c
	../external/lua/src/lcode.c
	../external/lua/src/lctype.c
	../external/lua/src/ldebug.c
	../external/lua/src/ldo.c
	../external/lua/src/ldump.c
	../external/lua/src/lfunc.c
	../external/lua/src/lgc.c
	../external/lua/src/llex.c
	../external/lua/src/lmem.c
	../external/lua/src/lobject.c
	../external/lua/src/lopcodes.c
	../external/lua/src/lparser.c
	../external/lua/src/lstate.c
	../external/lua/src/lstring.c
	../external/lua/src/ltable.c
	../external/lua/src/ltm.c
	../external/lua/src/lundump.c
	../external/lua/src/lvm.c
	../external/lua/src/lzio.c
	../external/lua/src/lauxlib.c
	../external/lua/src/lbaselib.c
	../external/lua/src/lbitlib.c
	../external/lua/src/lcorolib.c
	../external/lua/src/ldblib.c
	../external/lua/src/liolib.c
	../external/lua/src/lmathlib.c
	../external/lua/src/loslib.c
	../external/lua/src/lstrlib.c
	../external/lua/src/ltablib.c
	../external/lua/src/loadlib.c
	../external/lua/src/linit.c
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


add_executable(main WIN32 ${all_src})

if(USE_ASTYLE_BUILD)
	add_custom_command(TARGET main
					   PRE_BUILD
					   COMMAND astyle ARGS --options=${CMAKE_SOURCE_DIR}/astyle-options.txt ${game_src}
					   WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
					   COMMENT "Running astyle" VERBATIM)
endif(USE_ASTYLE_BUILD)

if(USE_CPPLINT_BUILD)
	add_custom_command(TARGET main
					   PRE_BUILD
					   COMMAND ./cpplint.py ARGS --output=vs7 ${game_src}
					   WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
					   COMMENT "Running CppLint" VERBATIM)
endif(USE_CPPLINT_BUILD)

if(USE_DOXYGEN_BUILD)
	add_custom_command(TARGET main
					   PRE_LINK
					   COMMAND ${DOXYGEN_EXECUTABLE} ARGS ${CMAKE_CURRENT_BINARY_DIR}/../Doxyfile
					   WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
					   COMMENT "Running Doxygen" VERBATIM)
	message(STATUS "Doxygen exe is ${DOXYGEN_EXECUTABLE} with args ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile in dir 			${CMAKE_SOURCE_DIR}/euphoria")
endif(USE_DOXYGEN_BUILD)

message(STATUS "Boost lib is ${Boost_LIBRARIES}")
message(STATUS "Sfml lib is ${SFML_LIBRARIES}")

# message(STATUS "Bullet is ${BULLET_LIBRARIES}")

# message(STATUS "Sources is ${game_src}")

# "../external/soil" 
include_directories("../external/glew" "../external/anttweakbar/include" ${SFML_INCLUDE_DIR} ${BULLET_INCLUDE_DIR})

target_link_libraries(main ${OPENGL_LIBRARIES} ${Boost_LIBRARIES} ${SFML_LIBRARIES} ${wxWidgets_LIBRARIES} ${ASSIMP_LIBRARIES} ${BULLET_LIBRARIES})
