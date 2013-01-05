#source: http://kollide.net:8060/browse/Amarok-git/cmake/modules/FindGooglemock.cmake
# - Find Googlemock
# Find the google mock includes and the google mock libraries
# This module defines
# GOOGLEMOCK_INCLUDE_DIRS, root google mock & gtest include dirs
# GOOGLEMOCK_LIBRARY, the path to Google Mock library
# GOOGLEMOCK_LIBRARIES, the path to Google Mock and Google Test library
# GOOGLEMOCK_FOUND, whether Google Mock was found

# update this to include gtest too?
# for now the fallback seems to work on ubuntu
#find_program(GMOCK-CONFIG_EXECUTABLE NAMES gmock-config PATHS
#       ${BIN_INSTALL_DIR}
#       /opt/local/bin
#       /usr/bin
#)

if(GMOCK-CONFIG_EXECUTABLE)
	exec_program(${GMOCK-CONFIG_EXECUTABLE} ARGS --includedir OUTPUT_VARIABLE GOOGLEMOCK_INCLUDE_DIR)
	exec_program(${GMOCK-CONFIG_EXECUTABLE} ARGS --ldflags OUTPUT_VARIABLE GOOGLEMOCK_LDFLAGS)
	exec_program(${GMOCK-CONFIG_EXECUTABLE} ARGS --libs OUTPUT_VARIABLE GOOGLEMOCK_libs_tmp)
	set(GOOGLEMOCK_LIBRARIES ${GOOGLEMOCK_LDFLAGS} ${GOOGLEMOCK_libs_tmp})

	if(GOOGLEMOCK_INCLUDE_DIR AND GOOGLEMOCK_LIBRARIES)
	   set(GOOGLEMOCK_FOUND TRUE)
	   message(STATUS "Found libgmock: ${GOOGLEMOCK_INCLUDE_DIR}, ${GOOGLEMOCK_LIBRARIES}")
	else(GOOGLEMOCK_INCLUDE_DIR AND GOOGLEMOCK_LIBRARIES)
	   set(GOOGLEMOCK_FOUND FALSE)
	   if (GOOGLEMOCK_FIND_REQUIRED)
		  message(FATAL_ERROR "Could NOT find required package Googlemock")
	   endif(GOOGLEMOCK_FIND_REQUIRED)
	endif(GOOGLEMOCK_INCLUDE_DIR AND GOOGLEMOCK_LIBRARIES)

else(GMOCK-CONFIG_EXECUTABLE)

	find_path(GOOGLEMOCK_INCLUDE_DIR NAMES gmock/gmock.h
	   HINTS
	   ~/usr/include
	   /opt/local/include
	   /usr/include
	   /usr/local/include
	   /opt/kde4/include
	   ${KDE4_INCLUDE_DIR}
	   $ENV{GMOCK_ROOT}/include
	)
	
	find_path(GOOGLEMOCK_GTEST_INCLUDE_DIR NAMES gtest/gtest.h
	   HINTS
	   ~/usr/include
	   /opt/local/include
	   /usr/include
	   /usr/local/include
	   /opt/kde4/include
	   ${KDE4_INCLUDE_DIR}
	   $ENV{GMOCK_ROOT}/gtest/include
	)

	find_library( GOOGLEMOCK_LIBRARY NAMES gmock
		PATHS
		~/usr/lib
	   /opt/local/lib
	   /usr/lib
	   /usr/lib64
	   /usr/local/lib
	   /opt/kde4/lib
	   ${KDE4_LIB_DIR}
	   $ENV{GMOCK_ROOT}/msvc/Release
	   $ENV{GMOCK_ROOT}/msvc/2005/Release
	   $ENV{GMOCK_ROOT}/msvc/2010/Release
	   $ENV{GMOCK_ROOT}/build/Release
	)
	
	if( WIN32 AND GOOGLEMOCK_LIBRARY)
		find_library( GOOGLEMOCK_LIBRARY_DEBUG NAMES gmockd gmock
			PATHS
			~/usr/lib
		   /opt/local/lib
		   /usr/lib
		   /usr/lib64
		   /usr/local/lib
		   /opt/kde4/lib
		   ${KDE4_LIB_DIR}
		   $ENV{GMOCK_ROOT}/msvc/Debug
		   $ENV{GMOCK_ROOT}/msvc/2005/Debug
		   $ENV{GMOCK_ROOT}/msvc/2010/Debug
		   $ENV{GMOCK_ROOT}/build/Debug
		)
		
		if( GOOGLEMOCK_LIBRARY_DEBUG )
			set(GOOGLEMOCK_LIBRARY debug ${GOOGLEMOCK_LIBRARY_DEBUG} optimized ${GOOGLEMOCK_LIBRARY})
		else(GOOGLEMOCK_LIBRARY_DEBUG )
			set(GOOGLEMOCK_LIBRARY "")
		endif(GOOGLEMOCK_LIBRARY_DEBUG)
	endif( WIN32 AND GOOGLEMOCK_LIBRARY)
	
	if( NOT WIN32 AND GOOGLEMOCK_LIBRARY)
		find_library( GOOGLEMOCK_TEST_LIBRARY NAMES gtest
			PATHS
			~/usr/lib
		   /opt/local/lib
		   /usr/lib
		   /usr/lib64
		   /usr/local/lib
		   /opt/kde4/lib
		   ${KDE4_LIB_DIR}
		)
		
		find_library( GOOGLEMOCK_PTHREAD_LIBRARY NAMES pthread
			PATHS
			~/usr/lib
		   /opt/local/lib
		   /usr/lib
		   /usr/lib64
		   /usr/local/lib
		   /opt/kde4/lib
		   ${KDE4_LIB_DIR}
		)
		
		if( GOOGLEMOCK_TEST_LIBRARY AND GOOGLEMOCK_PTHREAD_LIBRARY)
			set(GOOGLEMOCK_LIBRARY ${GOOGLEMOCK_PTHREAD_LIBRARY} ${GOOGLEMOCK_TEST_LIBRARY} ${GOOGLEMOCK_LIBRARY})
		else(GOOGLEMOCK_TEST_LIBRARY )
			set(GOOGLEMOCK_LIBRARY "")
		endif(GOOGLEMOCK_TEST_LIBRARY AND GOOGLEMOCK_PTHREAD_LIBRARY)
	endif( NOT WIN32 AND GOOGLEMOCK_LIBRARY)

	if( GMOCK_CMAKE_DEBUG )
		message("gmock root:  $ENV{GMOCK_ROOT}")
		message("gmock inc: ${GOOGLEMOCK_INCLUDE_DIR}")
		message("gmock lib: ${GOOGLEMOCK_LIBRARY}")
		message("gmocks gtest include dir: ${GOOGLEMOCK_GTEST_INCLUDE_DIR}")
	endif(GMOCK_CMAKE_DEBUG)

	if(GOOGLEMOCK_INCLUDE_DIR AND GOOGLEMOCK_LIBRARY AND GOOGLEMOCK_GTEST_INCLUDE_DIR)
	   set(GOOGLEMOCK_FOUND TRUE)
	   set(GOOGLEMOCK_LIBRARIES ${GOOGLEMOCK_LIBRARY})
	   set(GOOGLEMOCK_INCLUDE_DIRS ${GOOGLEMOCK_INCLUDE_DIR} ${GOOGLEMOCK_GTEST_INCLUDE_DIR})
	   # message(STATUS "Found libgmock: ${GOOGLEMOCK_INCLUDE_DIR}, ${GOOGLEMOCK_LIBRARIES}")
	else(GOOGLEMOCK_INCLUDE_DIR AND GOOGLEMOCK_LIBRARY  AND GOOGLEMOCK_GTEST_INCLUDE_DIR)
	   set(GOOGLEMOCK_FOUND FALSE)
	   if (GOOGLEMOCK_FIND_REQUIRED)
		  message(FATAL_ERROR "Could NOT find required package Googlemock")
	   endif(GOOGLEMOCK_FIND_REQUIRED)
	endif(GOOGLEMOCK_INCLUDE_DIR AND GOOGLEMOCK_LIBRARY  AND GOOGLEMOCK_GTEST_INCLUDE_DIR)

endif(GMOCK-CONFIG_EXECUTABLE)

mark_as_advanced(GOOGLEMOCK_INCLUDE_DIR GOOGLEMOCK_LIBRARIES)
