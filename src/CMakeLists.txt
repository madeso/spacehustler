FILE(GLOB game_src *.*)

set ( src_glew
	../external/glew/GL/glew.c
	../external/glew/GL/glew.h
	../external/glew/GL/wglew.h
)

source_group("" FILES ${game_src})
source_group(glew FILES ${src_glew})

add_executable(main WIN32 ${game_src} ${src_glew})

message(STATUS "Boost is ${Boost_LIBRARIES}")
message(STATUS "Sfml is ${SFML_LIBRARIES}")

include_directories("../external/glew" ${SFML_INCLUDE_DIR})

target_link_libraries(main ${OPENGL_LIBRARIES} ${Boost_LIBRARIES} ${SFML_LIBRARIES} ${wxWidgets_LIBRARIES})