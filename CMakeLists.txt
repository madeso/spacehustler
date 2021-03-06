cmake_minimum_required(VERSION 3.0.2)
project(euphoria)


set( CMAKE_VERBOSE_MAKEFILE on )

# these properties help when cmake can't find local data
#set(GMOCK_CMAKE_DEBUG ON)
#set(PHYSFS_DEBUG ON)
#set(ASSIMP_DEBUG True)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set_property(GLOBAL PROPERTY USE_FOLDERS OFF)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake-modules")

option(BUILD_USE_CLANG_FORMAT "Add clang format to the build" OFF)
option(BUILD_USE_CPPLINT "Add cpplint to the build" OFF)
option(BUILD_USE_DOXYGEN "Add doxygen to the build" OFF)
option(BUILD_USE_LUADOCGEN "parse all sources and build lua doc from it" OFF)
option(BUILD_USE_CPPCHECK "run cppcheck on all source" OFF)
option(BUILD_USE_CLOC "Update code stat files" OFF)
option(EUPHORIA_MESA_COMPABILITY "Mesa workaround" OFF)

option(USE_TWEAKABLES "Use tweakables" ON)
option(USE_OCULUS "Add oculus support" OFF)

# compiler flags
if(${MSVC})
	ADD_DEFINITIONS(/D _CRT_SECURE_NO_WARNINGS)
	
	# character set to Not Set=ASCII/SBCS http://public.kitware.com/Bug/view.php?id=12189
	ADD_DEFINITIONS(/D _SBCS)
endif(${MSVC})

# force visual studio and the like to only have debug and release builds
if(CMAKE_CONFIGURATION_TYPES)
	set(CMAKE_CONFIGURATION_TYPES Debug Release)
	set(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" CACHE STRING "Reset the configurations to what we need" FORCE)
endif()

# COMPONENTS filesystem program_options serialization system 
find_package(OpenGL REQUIRED)
find_package(Bullet REQUIRED)
find_package(AssImp REQUIRED)

# option(BUILD_SHARED_LIBS "build shared libraries" OFF)
set(BUILD_SHARED_LIBS OFF)

# add a target to generate API documentation with Doxygen
IF(BUILD_USE_DOXYGEN)
	find_package(Doxygen REQUIRED)
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.ini ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
ENDIF(BUILD_USE_DOXYGEN)

configure_file ("${PROJECT_SOURCE_DIR}/euphoria-config-in.h" "${PROJECT_BINARY_DIR}/euphoria-config.h")

include_directories("external")
include_directories("external/cml-1_0_3")
include_directories("external/jsoncpp")
include_directories("external/glew")
include_directories("external/anttweakbar/include")
include_directories("external/lua")

IF(USE_OCULUS)
	include_directories("external/LibOVR/Include")
ENDIF(USE_OCULUS)

include_directories(${ASSIMP_INCLUDE_DIR})
include_directories(${BULLET_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR})


message(STATUS "Including Assimp ${ASSIMP_INCLUDE_DIR}")
message(STATUS "Including Bullet ${BULLET_INCLUDE_DIR}")
message(STATUS "Including Cmake Source ${CMAKE_SOURCE_DIR}")

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(euphoria)

IF(BUILD_USE_CLOC)
	add_custom_target(codestat-everything ALL
					   cloc euphoria examples CMakeLists.txt --quiet --out code-stat-everything.txt
					   WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
					   COMMENT "Running cloc on everything" VERBATIM)

	add_custom_target(codestat-input-only ALL
					   cloc --by-file euphoria/input* --quiet --out code-stat-input.txt
					   WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
					   COMMENT "Running cloc on input only" VERBATIM)
ENDIF(BUILD_USE_CLOC)

IF(BUILD_USE_DOXYGEN)
	add_custom_target(doxygen ALL
						${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
						WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
						COMMENT "Running Doxygen" VERBATIM)

	add_custom_target(check-doxygen ALL
						${CMAKE_SOURCE_DIR}/check-doxygen-xml.py
						WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
						COMMENT "Checking doxygen xml against google cpp style" VERBATIM)
ENDIF(BUILD_USE_DOXYGEN)
