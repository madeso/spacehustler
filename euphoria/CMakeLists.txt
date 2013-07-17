FILE(GLOB game_src_glob *.cpp;*.h)
set(game_src ${game_src_glob} "${PROJECT_BINARY_DIR}/euphoria-config.h")

if(USE_TWEAKABLES)
	set ( src_anttweakbar
		../external/anttweakbar/src/LoadOGL.cpp
		../external/anttweakbar/src/LoadOGLCore.cpp
		../external/anttweakbar/src/TwEventSFML.cpp
		../external/anttweakbar/src/TwBar.cpp
		../external/anttweakbar/src/TwColors.cpp
		../external/anttweakbar/src/TwFonts.cpp
		../external/anttweakbar/src/TwMgr.cpp
		../external/anttweakbar/src/TwOpenGL.cpp
		../external/anttweakbar/src/TwOpenGLCore.cpp
		../external/anttweakbar/src/TwPrecomp.cpp
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

source_group("" FILES ${game_src})
source_group(glew FILES ${src_glew})
source_group(soil FILES ${src_soil})
source_group(jsoncpp FILES ${src_jsoncpp})
source_group(cmake FILES "${PROJECT_SOURCE_DIR}/euphoria-config-in.h")

set(all_src ${game_src} "${PROJECT_SOURCE_DIR}/euphoria-config-in.h" ${src_glew} ${src_soil} ${src_jsoncpp})

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
