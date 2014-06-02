# - Find the OpenGL Extension Wrangler Library (GLEW)
# This module defines the following variables:
#  GLEW_INCLUDE_DIRS - include directories for GLEW
#  GLEW_LIBRARIES - libraries to link against GLEW
#  GLEW_FOUND - true if GLEW has been found and can be used

#=============================================================================
# Copyright 2012 Benjamin Eikel
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
# (To distribute this file outside of CMake, substitute the full
#  License text for the above reference.)

set(GLEW_SEARCH_PATHS
	~/Library/Frameworks
	~/dev/fakeroot
	/Library/Frameworks
	/usr/local
	/usr
	/sw # Fink
	/opt/local # DarwinPorts
	/opt/csw # Blastwave
	/opt

	#Windows Search paths
	"C:/Program Files (x86)/"
	"C:/Program Files/"
	"$ENV{ProgramFiles}/"	

	"C:/fakeroot/"

	"C:/fakeroot/include"
	"C:/fakeroot/lib"
	"C:/dev/libs"
	)

find_path(GLEW_INCLUDE_DIR GL/glew.h ${GLEW_SEARCH_PATHS})
find_library(GLEW_LIBRARY NAMES GLEW glew32 glew glew32s PATH_SUFFIXES lib64 PATHS ${GLEW_SEARCH_PATHS})

set(GLEW_INCLUDE_DIRS ${GLEW_INCLUDE_DIR})
set(GLEW_LIBRARIES ${GLEW_LIBRARY})

include(${CMAKE_CURRENT_LIST_DIR}/FindPackageHandleStandardArgs.cmake)
find_package_handle_standard_args(GLEW
                                  REQUIRED_VARS GLEW_INCLUDE_DIR GLEW_LIBRARY)

mark_as_advanced(GLEW_INCLUDE_DIR GLEW_LIBRARY)
