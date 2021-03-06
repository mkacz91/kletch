# Locate anttweakbar library

# This module defines the following variables:
# ANTTWEAKBAR_LIBRARY, the name of the library;
# ANTTWEAKBAR_INCLUDE_DIR, where to find glfw include files.
# ANTTWEAKBAR_FOUND, true if both the ANTTWEAKBAR_LIBRARY and ANTTWEAKBAR_INCLUDE_DIR have been found.

#Search for the include file...
FIND_PATH(ANTTWEAKBAR_INCLUDE_DIR AntTweakBar.h
  HINTS ENV ANTTWEAKBAR_ROOT
  PATH_SUFFIXES include #For finding the include file under the root of the glfw expanded archive, typically on Windows.
  PATHS
  /usr/include/
  /usr/local/include/
  ${ANTTWEAKBAR_ROOT_DIR}/include/ # added by ptr
  ${CMAKE_SOURCE_DIR}/extlibs/headers
  /sw
  /opt/local
  /opt/csw
  /opt
)

SET(ANTTWEAKBAR_LIB_NAMES libAntTweakBar.dylib libAntTweakBar.a AntTweakBar.lib AntTweakBar)

FIND_LIBRARY(ANTTWEAKBAR_LIBRARY DOC "Absolute path to AntTweakBar library."
  NAMES ${ANTTWEAKBAR_LIB_NAMES}
  HINTS ENV ANTTWEAKBAR_ROOT
  PATH_SUFFIXES lib lib/win32 #For finding the library file under the root of the glfw expanded archive, typically on Windows.
  PATHS
  /usr/local/lib
  /usr/lib
  /sw
  /opt/local
  /opt/csw
  /opt
)

include("${CMAKE_ROOT}/Modules/FindPackageHandleStandardArgs.cmake")
find_package_handle_standard_args(ANTTWEAKBAR REQUIRED_VARS ANTTWEAKBAR_LIBRARY ANTTWEAKBAR_INCLUDE_DIR)
