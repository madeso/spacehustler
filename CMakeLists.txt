cmake_minimum_required (VERSION 2.6)
project(euphoria)

# these properties help when cmake can't find local data
#set(GMOCK_CMAKE_DEBUG ON)
#set(PHYSFS_DEBUG ON)
set(ASSIMP_DEBUG True)

set_property(GLOBAL PROPERTY USE_FOLDERS OFF)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake-modules")

option(USE_ASTYLE_BUILD "Add astyle to the build" ON)
option(USE_CPPLINT_BUILD "Add cpplint to the build" ON)
option(USE_DOXYGEN_BUILD "Add doxygen to the build" ON)

option(USE_TWEAKABLES "Use tweakables" ON)

if(WIN32)
	#on windows, link to sfml static, to avoid crash on exit with ati cards
	set(SFML_STATIC_LIBRARIES TRUE)
	message(STATUS "using static sfml libraries")
else(WIN32)
	#on linux, link with dynamic since static library dependencies are horrible on linux
	#set(SFML_STATIC_LIBRARIES OFF)
	message(STATUS "using non-static libraries")
	
	#also add c++11 flags
	SET(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++ ${CMAKE_CXX_FLAGS}")
endif(WIN32)

IF(WIN32)
	ADD_DEFINITIONS(/D _CRT_SECURE_NO_WARNINGS)
ENDIF(WIN32)

# force visual studio and the like to only have debug and release builds
if(CMAKE_CONFIGURATION_TYPES)
	set(CMAKE_CONFIGURATION_TYPES Debug Release)
	set(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" CACHE STRING "Reset the configurations to what we need" FORCE)
 endif()

# COMPONENTS filesystem program_options serialization system 
find_package(OpenGL REQUIRED)
find_package(Bullet REQUIRED)
find_package(wxWidgets REQUIRED core base)
include( "${wxWidgets_USE_FILE}" )
find_package(AssImp REQUIRED)

#on windows main is needed, not on linux
if(WIN32)
	find_package(SFML 2 COMPONENTS system window main REQUIRED)
else(WIN32)
	find_package(SFML 2 COMPONENTS system window REQUIRED)
endif(WIN32)

option(BUILD_SHARED_LIBS "build shared libraries" OFF)
set(BUILD_SHARED_LIBS OFF)

# add a target to generate API documentation with Doxygen
find_package(Doxygen REQUIRED)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.ini ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)

configure_file ("${PROJECT_SOURCE_DIR}/euphoria-config-in.h" "${PROJECT_BINARY_DIR}/euphoria-config.h")

include_directories(external)
include_directories(external/cml-1_0_3)
include_directories(external/jsoncpp)
include_directories(${CMAKE_SOURCE_DIR})
include_directories(${ASSIMP_INCLUDE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

message(STATUS "Bullet include is ${BULLET_INCLUDE_DIR}" )

add_subdirectory(euphoria)
