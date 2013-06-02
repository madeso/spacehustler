FILE(GLOB game_src *.cpp;*.h)

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

add_executable(main WIN32 ${game_src} ${src_glew} ${src_soil})

add_custom_command(TARGET main
                   PRE_BUILD
                   COMMAND astyle ARGS --options=${CMAKE_SOURCE_DIR}/astyle-options.txt ${game_src}
				   WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                   COMMENT "Running astyle" VERBATIM)
add_custom_command(TARGET main
                   PRE_BUILD
                   COMMAND cpplint.py ARGS --output=vs7 ${game_src}
				   WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                   COMMENT "Running CppLint" VERBATIM)

add_custom_command(TARGET main
				   PRE_LINK
				   COMMAND ${DOXYGEN_EXECUTABLE} ARGS ${CMAKE_CURRENT_BINARY_DIR}/../Doxyfile
				   WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
				   COMMENT "Running Doxygen" VERBATIM)
message(STATUS "Doxygen exe is ${DOXYGEN_EXECUTABLE} with args ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile in dir ${CMAKE_SOURCE_DIR}/euphoria")

message(STATUS "Boost lib is ${Boost_LIBRARIES}")
message(STATUS "Sfml lib is ${SFML_LIBRARIES}")

message(STATUS "Sources is ${game_src}")

# "../external/soil" 
include_directories("../external/glew" ${SFML_INCLUDE_DIR})

target_link_libraries(main ${OPENGL_LIBRARIES} ${Boost_LIBRARIES} ${SFML_LIBRARIES} ${wxWidgets_LIBRARIES} ${ASSIMP_LIBRARY})