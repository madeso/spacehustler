FILE(GLOB game_src_glob *.cc;*.h;*.inl)
FILE(GLOB graphics_src_glob graphics/*.cc;graphics/*.h;graphics/*.inl)
FILE(GLOB game_input_src_glob input*.cc;input*.h)
FILE(GLOB game_ui_src_glob ui-*.cc;ui-*.h)
set(game_src ${game_src_glob} "${PROJECT_BINARY_DIR}/euphoria-config.h" ${graphics_src_glob})

#libovr requires unicode support
add_definitions(-DUNICODE -D_UNICODE)

INCLUDE(LibOVR.cmake)

if(USE_TWEAKABLES)
	set ( src_dearimgui
		../external/imgui/imconfig.h
		../external/imgui/imgui.cpp
		../external/imgui/imgui.h
		../external/imgui/imgui_demo.cpp
		../external/imgui/imgui_draw.cpp
		../external/imgui/imgui_internal.h
		../external/imgui/stb_rect_pack.h
		../external/imgui/stb_textedit.h
		../external/imgui/stb_truetype.h
		../external/imgui/examples/sdl_opengl3_example/imgui_impl_sdl_gl3.cpp
		../external/imgui/examples/sdl_opengl3_example/imgui_impl_sdl_gl3.h
	)
	ADD_DEFINITIONS( -DTW_STATIC )
endif(USE_TWEAKABLES)

set ( src_jsoncpp
	../external/jsoncpp/json_reader.cpp
	../external/jsoncpp/json_value.cpp
	../external/jsoncpp/json_writer.cpp
)

# util executables
# ../external/glew/GL/visualinfo.c
# ../external/glew/GL/glewinfo.c
set ( src_glew
	../external/glew/GL/eglew.h
	../external/glew/GL/glew.c
	../external/glew/GL/glew.h
	../external/glew/GL/glxew.h
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
source_group(graphics FILES ${graphics_src_glob})
source_group(lua FILES ${src_lua})
source_group(jsoncpp FILES ${src_jsoncpp})
source_group(cmake FILES "${PROJECT_SOURCE_DIR}/euphoria-config-in.h")
source_group(input FILES ${game_input_src_glob})
source_group(ui FILES ${game_ui_src_glob})

set(all_src ${game_src} "${PROJECT_SOURCE_DIR}/euphoria-config-in.h" ${src_glew} ${src_soil} ${src_jsoncpp} ${src_lua})

if(USE_TWEAKABLES)
	source_group("dear imgui" FILES ${src_dearimgui})
	set(all_src ${all_src} ${src_dearimgui})
endif(USE_TWEAKABLES)

set(extra_libraries)

if(USE_OCULUS)
	set(all_src ${all_src} ${src_oculus})
	add_definitions( -DOVR_DISABLE_D3D ) # my modification to the libovr, disable d3d
	
	IF(WIN32)
	# Dbghelp.lib libovrd.lib dxgi.lib dxguid.lib d3d10.lib d3d11.lib d3dcompiler.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib
		set(extra_libraries ${extra_libraries} setupapi.lib ws2_32.lib winmm.lib)
	ENDIF(WIN32)
endif(USE_OCULUS)

find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIR})

source_group("" FILES "main/main-sdl2.cc")

add_executable(main-sdl2 WIN32 ${all_src} "main/main-sdl2.cc")

GET_FILENAME_COMPONENT(mainsrc "main/main-sdl2.cc" ABSOLUTE)

if(BUILD_USE_CLANG_FORMAT)

	add_custom_command(TARGET main-sdl2
				   PRE_BUILD
				   COMMAND ./call-clang-format.py ${CMAKE_CURRENT_SOURCE_DIR}/*.cc ${CMAKE_CURRENT_SOURCE_DIR}/*.h
				   WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
				   COMMENT "Running clang format" VERBATIM)
endif(BUILD_USE_CLANG_FORMAT)

if(BUILD_USE_CPPLINT)
	add_custom_command(TARGET main-sdl2
					   PRE_BUILD
					   COMMAND ./cpplint.py ARGS --output=vs7 ${CMAKE_CURRENT_SOURCE_DIR}/*.cc ${CMAKE_CURRENT_SOURCE_DIR}/*.h  ${mainsrc}
					   WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
					   COMMENT "Running CppLint" VERBATIM)
endif(BUILD_USE_CPPLINT)

if(BUILD_USE_CPPCHECK)
	add_custom_command(TARGET main-sdl2
					   PRE_BUILD
					   COMMAND cppcheck --enable=all --std=c++11 --suppress=unusedFunction --quiet --template=vs ${CMAKE_SOURCE_DIR}/euphoria
					   WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
					   COMMENT "Running cppcheck" VERBATIM)
endif(BUILD_USE_CPPCHECK)

if(BUILD_USE_DOXYGEN)
	add_custom_command(TARGET main-sdl2
					   PRE_LINK
					   COMMAND ${DOXYGEN_EXECUTABLE} ARGS ${CMAKE_CURRENT_BINARY_DIR}/../Doxyfile
					   WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
					   COMMENT "Running Doxygen" VERBATIM)
endif(BUILD_USE_DOXYGEN)

if(BUILD_USE_LUADOCGEN)
	add_custom_command(TARGET main-sdl2
					   PRE_LINK
					   COMMAND ./luadocgen.py ARGS luadoc/ ${game_src}
					   WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
					   COMMENT "Running luadocgen" VERBATIM)
endif(BUILD_USE_LUADOCGEN)

message(STATUS "Linking with launcher sdl2 ${SDL2_LIBRARY}")

target_link_libraries(main-sdl2 ${BULLET_LIBRARIES} ${extra_libraries} ${OPENGL_LIBRARIES} ${SDL2_LIBRARY} ${ASSIMP_LIBRARIES})

message(STATUS "Linking with OpenGL ${OPENGL_LIBRARIES}")
message(STATUS "Linking with AssImp ${ASSIMP_LIBRARIES}")
message(STATUS "Linking with Bullet ${BULLET_LIBRARIES}")
message(STATUS "Linking with extra libs ${extra_libraries}")

if(BUILD_USE_DOXYGEN)
	message(STATUS "Doxygen exe is ${DOXYGEN_EXECUTABLE} with args ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile in dir ${CMAKE_SOURCE_DIR}/euphoria")
endif(BUILD_USE_DOXYGEN)