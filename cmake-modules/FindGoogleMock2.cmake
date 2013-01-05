#source: http://gja.in/pub/FindGoogleMock.cmake
# Locate and configure the Google Mock libraries.
#
# Defines the following variable:
#
#   GMOCK_FOUND - Found the Google Mock libraries
#   GMOCK_INCLUDE_DIRS - The directories needed on the include paths
#   GMOCK_LIBRARIES - The libraries to link to mock executables
#   GMOCK_MAIN_LIBRARIES - The libraries to link for automatic main() provision
#
#	Copyright 2008 Chandler Carruth
#
#	Licensed under the Apache License, Version 2.0 (the "License"); you may not
#	use this file except in compliance with the License.  You may obtain a copy
#	of the License at
#
#		http://www.apache.org/licenses/LICENSE-2.0
#
#	Unless required by applicable law or agreed to in writing, software
#	distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
#	WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
#	License for the specific language governing permissions and limitations
#	under the License.


if(GMOCK_INCLUDE_DIRS AND GMOCK_LIBRARIES AND GMOCK_MAIN_LIBRARIES)
  set(GMOCK_FOUND true)
else(GMOCK_INCLUDE_DIRS AND GMOCK_LIBRARIES AND GMOCK_MAIN_LIBRARIES)
  set(GMOCK_PREFIX "" CACHE PATH "Installation prefix for Google Mock")
  if(GMOCK_PREFIX)
    find_path(_GMOCK_INCLUDE_DIR gmock.h
      PATHS "${GMOCK_PREFIX}/include"
      PATH_SUFFIXES "gmock"
      NO_DEFAULT_PATH)
    find_library(_GMOCK_LIBRARY gmock
      PATHS "${GMOCK_PREFIX}/lib"
      NO_DEFAULT_PATH)
    find_library(_GMOCK_MAIN_LIBRARY gmock_main
      PATHS "${GMOCK_PREFIX}/lib"
      NO_DEFAULT_PATH)
  else(GMOCK_PREFIX)
    find_path(_GMOCK_INCLUDE_DIR gmock.h
      PATH_SUFFIXES "gmock")
    find_library(_GMOCK_LIBRARY gmock)
    find_library(_GMOCK_MAIN_LIBRARY gmock_main)
  endif(GMOCK_PREFIX)
  if(_GMOCK_INCLUDE_DIR AND _GMOCK_LIBRARY AND _GMOCK_MAIN_LIBRARY)
    set(GMOCK_FOUND true)
    set(GMOCK_INCLUDE_DIRS ${_GMOCK_INCLUDE_DIR} CACHE PATH
      "Include directories for Google Mock framework")
    set(GMOCK_LIBRARIES ${_GMOCK_LIBRARY} CACHE FILEPATH
      "Libraries to link for Google Mock framework")
    set(GMOCK_MAIN_LIBRARIES ${_GMOCK_MAIN_LIBRARY} CACHE FILEPATH
      "Libraries to link for Google Mock automatic main() definition")
    mark_as_advanced(GMOCK_INCLUDE_DIRS GMOCK_LIBRARIES GMOCK_MAIN_LIBRARIES)
    if(NOT GoogleMock_FIND_QUIETLY)
      message(STATUS "Found Google Mock: ${GMOCK_LIBRARIES}")
    endif(NOT GoogleMock_FIND_QUIETLY)
  else(_GMOCK_INCLUDE_DIR AND _GMOCK_LIBRARY AND _GMOCK_MAIN_LIBRARY)
    if(GoogleMock_FIND_REQUIRED)
      message(FATAL_ERROR "Could not find the Google Mock framework")
    endif(GoogleMock_FIND_REQUIRED)
  endif(_GMOCK_INCLUDE_DIR AND _GMOCK_LIBRARY AND _GMOCK_MAIN_LIBRARY)
endif(GMOCK_INCLUDE_DIRS AND GMOCK_LIBRARIES AND GMOCK_MAIN_LIBRARIES)