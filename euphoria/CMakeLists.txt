FILE(GLOB game_src_glob *.cc;*.h)
FILE(GLOB game_input_src_glob input*.cc;input*.h)
set(game_src ${game_src_glob} "${PROJECT_BINARY_DIR}/euphoria-config.h")

if(USE_TWEAKABLES)
	set ( src_anttweakbar
		../external/anttweakbar/src/TwBar.cpp
		../external/anttweakbar/src/TwBar.h
		../external/anttweakbar/src/TwColors.cpp
		../external/anttweakbar/src/TwColors.h
		../external/anttweakbar/src/TwFonts.cpp
		../external/anttweakbar/src/TwFonts.h
		../external/anttweakbar/src/TwGraph.h
		../external/anttweakbar/src/TwMgr.cpp
		../external/anttweakbar/src/TwMgr.h
		../external/anttweakbar/src/TwOpenGLCore.cpp
		../external/anttweakbar/src/TwOpenGLCore.h
		../external/anttweakbar/src/TwPrecomp.h
	)
	ADD_DEFINITIONS( -DTW_STATIC )
endif(USE_TWEAKABLES)

if(USE_OCULUS)
	set ( src_oculus_include
		../external/LibOvr/Include/OVR.h
		../external/LibOvr/Include/OVRVersion.h
	)
	set ( src_oculus_src_core
		../external/LibOvr/Src/OVR_DeviceHandle.cpp
		../external/LibOvr/Src/OVR_DeviceImpl.cpp
		../external/LibOvr/Src/OVR_JSON.cpp
		../external/LibOvr/Src/OVR_LatencyTestImpl.cpp
		../external/LibOvr/Src/OVR_Profile.cpp
		../external/LibOvr/Src/OVR_SensorFilter.cpp
		../external/LibOvr/Src/OVR_SensorFusion.cpp
		../external/LibOvr/Src/OVR_SensorImpl.cpp
		../external/LibOvr/Src/OVR_ThreadCommandQueue.cpp
		../external/LibOvr/Src/OVR_Device.h
		../external/LibOvr/Src/OVR_DeviceConstants.h
		../external/LibOvr/Src/OVR_DeviceHandle.h
		../external/LibOvr/Src/OVR_DeviceImpl.h
		../external/LibOvr/Src/OVR_DeviceMessages.h
		../external/LibOvr/Src/OVR_HIDDevice.h
		../external/LibOvr/Src/OVR_HIDDeviceBase.h
		../external/LibOvr/Src/OVR_HIDDeviceImpl.h
		../external/LibOvr/Src/OVR_JSON.h
		../external/LibOvr/Src/OVR_LatencyTestImpl.h
		../external/LibOvr/Src/OVR_Profile.h
		../external/LibOvr/Src/OVR_SensorFilter.h
		../external/LibOvr/Src/OVR_SensorFusion.h
		../external/LibOvr/Src/OVR_SensorImpl.h
		../external/LibOvr/Src/OVR_ThreadCommandQueue.h
	)
	set ( src_oculus_src_linux
		../external/LibOvr/Src/OVR_Linux_DeviceManager.cpp
		../external/LibOvr/Src/OVR_Linux_HIDDevice.cpp
		../external/LibOvr/Src/OVR_Linux_HMDDevice.cpp
		../external/LibOvr/Src/OVR_Linux_SensorDevice.cpp
		../external/LibOvr/Src/OVR_Linux_DeviceManager.h
		../external/LibOvr/Src/OVR_Linux_HIDDevice.h
		../external/LibOvr/Src/OVR_Linux_HMDDevice.h
	)
	set ( src_oculus_src_osx
		../external/LibOvr/Src/OVR_OSX_DeviceManager.cpp
		../external/LibOvr/Src/OVR_OSX_HIDDevice.cpp
		../external/LibOvr/Src/OVR_OSX_HMDDevice.cpp
		../external/LibOvr/Src/OVR_OSX_SensorDevice.cpp
		../external/LibOvr/Src/OVR_OSX_DeviceManager.h
		../external/LibOvr/Src/OVR_OSX_HIDDevice.h
		../external/LibOvr/Src/OVR_OSX_HMDDevice.h
	)
	set ( src_oculus_src_win32
		../external/LibOvr/Src/OVR_Win32_DeviceManager.cpp
		../external/LibOvr/Src/OVR_Win32_DeviceStatus.cpp
		../external/LibOvr/Src/OVR_Win32_HIDDevice.cpp
		../external/LibOvr/Src/OVR_Win32_HMDDevice.cpp
		../external/LibOvr/Src/OVR_Win32_SensorDevice.cpp
		../external/LibOvr/Src/OVR_Win32_DeviceManager.h
		../external/LibOvr/Src/OVR_Win32_DeviceStatus.h
		../external/LibOvr/Src/OVR_Win32_HIDDevice.h
		../external/LibOvr/Src/OVR_Win32_HMDDevice.h
		../external/LibOvr/Src/OVR_Win32_SensorDevice.h
	)
	IF(WIN32)
		set (src_oculus_src ${src_oculus_src_core} ${src_oculus_src_win32})
	ELSE(WIN32)
		# matches darwin == osx
		IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
			set (src_oculus_src ${src_oculus_src_core} ${src_oculus_src_osx})
		ELSE(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
			set (src_oculus_src ${src_oculus_src_core} ${src_oculus_src_linux})
		ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	ENDIF(WIN32)
	set ( src_oculus_kernel_core
		../external/LibOvr/Src/Kernel/OVR_Alg.cpp
		../external/LibOvr/Src/Kernel/OVR_Allocator.cpp
		../external/LibOvr/Src/Kernel/OVR_Atomic.cpp
		../external/LibOvr/Src/Kernel/OVR_File.cpp
		../external/LibOvr/Src/Kernel/OVR_FileFILE.cpp
		../external/LibOvr/Src/Kernel/OVR_Log.cpp
		../external/LibOvr/Src/Kernel/OVR_Math.cpp
		../external/LibOvr/Src/Kernel/OVR_RefCount.cpp
		../external/LibOvr/Src/Kernel/OVR_Std.cpp
		../external/LibOvr/Src/Kernel/OVR_String.cpp
		../external/LibOvr/Src/Kernel/OVR_String_FormatUtil.cpp
		../external/LibOvr/Src/Kernel/OVR_String_PathUtil.cpp
		../external/LibOvr/Src/Kernel/OVR_SysFile.cpp
		../external/LibOvr/Src/Kernel/OVR_System.cpp
		../external/LibOvr/Src/Kernel/OVR_Timer.cpp
		../external/LibOvr/Src/Kernel/OVR_UTF8Util.cpp
		../external/LibOvr/Src/Kernel/OVR_Alg.h
		../external/LibOvr/Src/Kernel/OVR_Allocator.h
		../external/LibOvr/Src/Kernel/OVR_Array.h
		../external/LibOvr/Src/Kernel/OVR_Atomic.h
		../external/LibOvr/Src/Kernel/OVR_Color.h
		../external/LibOvr/Src/Kernel/OVR_ContainerAllocator.h
		../external/LibOvr/Src/Kernel/OVR_File.h
		../external/LibOvr/Src/Kernel/OVR_Hash.h
		../external/LibOvr/Src/Kernel/OVR_KeyCodes.h
		../external/LibOvr/Src/Kernel/OVR_List.h
		../external/LibOvr/Src/Kernel/OVR_Log.h
		../external/LibOvr/Src/Kernel/OVR_Math.h
		../external/LibOvr/Src/Kernel/OVR_RefCount.h
		../external/LibOvr/Src/Kernel/OVR_Std.h
		../external/LibOvr/Src/Kernel/OVR_String.h
		../external/LibOvr/Src/Kernel/OVR_StringHash.h
		../external/LibOvr/Src/Kernel/OVR_SysFile.h
		../external/LibOvr/Src/Kernel/OVR_System.h
		../external/LibOvr/Src/Kernel/OVR_Threads.h
		../external/LibOvr/Src/Kernel/OVR_Timer.h
		../external/LibOvr/Src/Kernel/OVR_Types.h
		../external/LibOvr/Src/Kernel/OVR_UTF8Util.h
	)
	set(src_oculus_kernel_win32
		../external/LibOvr/Src/Kernel/OVR_ThreadsWinAPI.cpp
	)
	set(src_oculus_kernel_nonwin32
		../external/LibOvr/Src/Kernel/OVR_ThreadsPthread.cpp
	)
	IF(WIN32)
		set (src_oculus_kernel ${src_oculus_kernel_core} ${src_oculus_kernel_win32})
	ELSE(WIN32)
		set (src_oculus_kernel_core ${src_oculus_kernel_core} ${src_oculus_kernel_nonwin32})
	ENDIF(WIN32)
	set ( src_oculus_util
		../external/LibOvr/Src/Util/Util_LatencyTest.cpp
		../external/LibOvr/Src/Util/Util_MagCalibration.cpp
		../external/LibOvr/Src/Util/Util_Render_Stereo.cpp
		../external/LibOvr/Src/Util/Util_LatencyTest.h
		../external/LibOvr/Src/Util/Util_MagCalibration.h
		../external/LibOvr/Src/Util/Util_Render_Stereo.h
	)
	set( src_oculus ${src_oculus_include} ${src_oculus_src} ${src_oculus_kernel} ${src_oculus_util} )
endif(USE_OCULUS)

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
source_group(input FILES ${game_input_src_glob})

set(all_src ${game_src} "${PROJECT_SOURCE_DIR}/euphoria-config-in.h" ${src_glew} ${src_soil} ${src_jsoncpp} ${src_lua})

if(USE_TWEAKABLES)
	source_group(anttweakbar FILES ${src_anttweakbar})
	set(all_src ${all_src} ${src_anttweakbar})
endif(USE_TWEAKABLES)

set(extra_libraries)

if(USE_OCULUS)
	source_group(oculusvr FILES ${src_oculus_include})
	source_group(oculusvr\\src FILES ${src_oculus_src})
	source_group(oculusvr\\src\\kernel FILES ${src_oculus_kernel})
	source_group(oculusvr\\src\\util FILES ${src_oculus_util})
	set(all_src ${all_src} ${src_oculus})
	
	IF(WIN32)
		set(extra_libraries ${extra_libraries} setupapi.lib)
	ENDIF(WIN32)
endif(USE_OCULUS)

MACRO(GenerateMain LauncherName TargetName LauncherIncludes LauncherLibraries)
	include_directories(${LauncherIncludes})
	message(STATUS "Including launcher ${LauncherName} ${LauncherIncludes}")

	source_group("" FILES "main/main-${LauncherName}.cc")
	
	add_executable(${TargetName} WIN32 ${all_src} "main/main-${LauncherName}.cc")
	
	GET_FILENAME_COMPONENT(mainsrc "main/main-${LauncherName}.cc" ABSOLUTE)

	if(BUILD_USE_ASTYLE)
		add_custom_command(TARGET ${TargetName}
						   PRE_BUILD
						   COMMAND astyle ARGS --options=${CMAKE_SOURCE_DIR}/astyle-options.txt ${game_src} ${mainsrc}
						   WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
						   COMMENT "Running astyle" VERBATIM)
	endif(BUILD_USE_ASTYLE)
	
	if(BUILD_USE_CLANG_FORMAT)
		add_custom_command(TARGET ${TargetName}
					   PRE_BUILD
					   COMMAND clang-format ARGS -i -style=Google ${game_src}
					   WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
					   COMMENT "Running clang format" VERBATIM)
	endif(BUILD_USE_CLANG_FORMAT)

	if(BUILD_USE_CPPLINT)
		add_custom_command(TARGET ${TargetName}
						   PRE_BUILD
						   COMMAND ./cpplint.py ARGS --output=vs7 ${CMAKE_CURRENT_SOURCE_DIR}/*.cc ${CMAKE_CURRENT_SOURCE_DIR}/*.h  ${mainsrc}
						   WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
						   COMMENT "Running CppLint" VERBATIM)
	endif(BUILD_USE_CPPLINT)
	
	if(BUILD_USE_CPPCHECK)
		add_custom_command(TARGET ${TargetName}
						   PRE_BUILD
						   COMMAND cppcheck --enable=all --std=c++11 --suppress=unusedFunction --quiet --template=vs ${CMAKE_SOURCE_DIR}/euphoria
						   WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
						   COMMENT "Running cppcheck" VERBATIM)
	endif(BUILD_USE_CPPCHECK)

	if(BUILD_USE_DOXYGEN)
		add_custom_command(TARGET ${TargetName}
						   PRE_LINK
						   COMMAND ${DOXYGEN_EXECUTABLE} ARGS ${CMAKE_CURRENT_BINARY_DIR}/../Doxyfile
						   WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
						   COMMENT "Running Doxygen" VERBATIM)
	endif(BUILD_USE_DOXYGEN)

	if(BUILD_USE_LUADOCGEN)
		add_custom_command(TARGET ${TargetName}
						   PRE_LINK
						   COMMAND ./luadocgen.py ARGS luadoc/ ${game_src}
						   WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
						   COMMENT "Running luadocgen" VERBATIM)
	endif(BUILD_USE_LUADOCGEN)

	message(STATUS "Linking with launcher ${LauncherName} ${LauncherLibraries}")

	target_link_libraries(${TargetName} ${OPENGL_LIBRARIES} ${LauncherLibraries} ${ASSIMP_LIBRARIES} ${BULLET_LIBRARIES} ${extra_libraries})
ENDMACRO(GenerateMain)

message(STATUS "Linking with OpenGL ${OPENGL_LIBRARIES}")
message(STATUS "Linking with AssImp ${ASSIMP_LIBRARIES}")
message(STATUS "Linking with Bullet ${BULLET_LIBRARIES}")

if(BUILD_USE_DOXYGEN)
	message(STATUS "Doxygen exe is ${DOXYGEN_EXECUTABLE} with args ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile in dir ${CMAKE_SOURCE_DIR}/euphoria")
endif(BUILD_USE_DOXYGEN)

#	Legacy launcher, not used anymore
#	option(USE_MAIN_SFML "Use sfml as launch library" ON)
#	if(USE_MAIN_SFML)
#		if(WIN32)
#			#on windows, link to sfml static, to avoid crash on exit with ati cards
#			set(SFML_STATIC_LIBRARIES TRUE)
#			message(STATUS "using static sfml libraries")
#		else(WIN32)
#			#on linux, link with dynamic since static library dependencies are horrible on linux
#			message(STATUS "using non-static libraries")
#		endif(WIN32)
#	
#		#on windows main is needed, not on linux
#		if(WIN32)
#			find_package(SFML 2 COMPONENTS system window main REQUIRED)
#		else(WIN32)
#			find_package(SFML 2 COMPONENTS system window REQUIRED)
#		endif(WIN32)
#		
#		GenerateMain("sfml" main-sfml "${SFML_INCLUDE_DIR}" "${SFML_LIBRARIES}")
#	endif(USE_MAIN_SFML)

option(USE_MAIN_SDL2 "Use sdl2 as launch library" ON)
if(USE_MAIN_SDL2)
	find_package(SDL2 REQUIRED)
	GenerateMain("sdl2" main-sdl2 "${SDL2_INCLUDE_DIR}" "${SDL2_LIBRARY}")
endif(USE_MAIN_SDL2)