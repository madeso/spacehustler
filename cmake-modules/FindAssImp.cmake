# Based on the FindPhysFS.cmake scipt
# - Try to find Assimp
# Once done this will define
#
#  ASSIMP_FOUND - system has Assimp
#  ASSIMP_INCLUDE_DIR - the Assimp include directory
#  ASSIMP_LIBRARY - Link these to use Assimp

set(_assimp_INCLUDE_SEARCH_DIRS_SYSTEM
    C:/assimp/include
    C:/assimp
    "$ENV{ProgramFiles}/assimp/include"
    "$ENV{ProgramFiles}/assimp"
    /sw/local/include
  )

set(_assimp_LIB_SEARCH_DIRS_SYSTEM
    C:/assimp/lib
    C:/assimp
    "$ENV{ProgramFiles}/assimp/lib"
    "$ENV{ProgramFiles}/assimp"
    /sw/local/lib
  )

FIND_PATH(ASSIMP_INCLUDE_DIR assimp/ai_assert.h
	$ENV{ASSIMPSDIR}/include
	$ENV{ASSIMPSDIR}
	$ENV{ASSIMPSDIR}/..
	~/Library/Frameworks/AssImp.framework/Headers
	/Library/Frameworks/AssImp.framework/Headers
	/usr/local/include/assimp
	/usr/local/include
	/usr/include/assimp
	/usr/include
	/sw/include/assimp # Fink
	/sw/include
	/opt/local/include/assimp # DarwinPorts
	/opt/local/include
	/opt/csw/include/assimp # Blastwave
	/opt/csw/include
	/opt/include/assimp
	/opt/include
	${_assimp_INCLUDE_SEARCH_DIRS_SYSTEM}
	)

FIND_LIBRARY(ASSIMP_LIBRARY_DEBUG 
	NAMES assimpD
	PATHS
	$ENV{ASSIMPSDIR}/lib
	/usr/local/lib
	/usr/lib
	/sw/lib
	/opt/local/lib
	/opt/csw/lib
	/opt/lib
	${_assimp_LIB_SEARCH_DIRS_SYSTEM}
	)

FIND_LIBRARY(ASSIMP_LIBRARY_RELEASE 
	NAMES assimp
	PATHS
	$ENV{ASSIMPSDIR}/lib
	/usr/local/lib
	/usr/lib
	/sw/lib
	/opt/local/lib
	/opt/csw/lib
	/opt/lib
	${_assimp_LIB_SEARCH_DIRS_SYSTEM}
	)

message(STATUS "assimp debug: ${ASSIMP_LIBRARY_DEBUG}")
message(STATUS "assimp release: ${ASSIMP_LIBRARY_RELEASE}")

SET(ASSIMP_FOUND "NO")
IF(WIN32)
	IF(ASSIMP_INCLUDE_DIR AND ASSIMP_LIBRARY_RELEASE)
		SET(ASSIMP_FOUND "YES")
		IF(ASSIMP_LIBRARY_DEBUG)
			SET(ASSIMP_LIBRARIES debug ${ASSIMP_LIBRARY_DEBUG} optimized ${ASSIMP_LIBRARY_RELEASE})
		ELSE(ASSIMP_LIBRARY_DEBUG)
			SET(ASSIMP_LIBRARIES ${ASSIMP_LIBRARY_RELEASE})
		ENDIF(ASSIMP_LIBRARY_DEBUG)
	ENDIF(ASSIMP_INCLUDE_DIR AND ASSIMP_LIBRARY_RELEASE)
ELSE(WIN32)
	# for linux there is only the release build
	IF(ASSIMP_INCLUDE_DIR AND ASSIMP_LIBRARY_RELEASE)
		SET(ASSIMP_FOUND "YES")
		SET(ASSIMP_LIBRARIES ${ASSIMP_LIBRARY_RELEASE})
	ENDIF(ASSIMP_INCLUDE_DIR AND ASSIMP_LIBRARY_RELEASE)
ENDIF(WIN32)

if(ASSIMP_DEBUG)
	message(STATUS "assimp inc: ${ASSIMP_INCLUDE_DIR}")
	message(STATUS "assimp lib: ${ASSIMP_LIBRARIES}")
ENDIF(ASSIMP_DEBUG)

if(AssImp_FIND_REQUIRED AND NOT (ASSIMP_LIBRARIES AND ASSIMP_INCLUDE_DIR))
	message(FATAL_ERROR "Could not find assimp")
ENDIF(AssImp_FIND_REQUIRED AND NOT (ASSIMP_LIBRARIES AND ASSIMP_INCLUDE_DIR))

mark_as_advanced(ASSIMP_LIBRARIES_DEBUG ASSIMP_LIBRARIES_RELEASE ASSIMP_INCLUDE_DIR)