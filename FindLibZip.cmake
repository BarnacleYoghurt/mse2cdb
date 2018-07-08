# Copyright (C) 2007-2009 LuaDist.
# Created by Peter Kapec <kapecp@gmail.com>
# Redistribution and use of this file is allowed according to the terms of the MIT license.
# For details see the COPYRIGHT file distributed with LuaDist.
#	Note:
#		Searching headers and libraries is very simple and is NOT as powerful as scripts
#		distributed with CMake, because LuaDist defines directories to search for.
#		Everyone is encouraged to contact the author with improvements. Maybe this file
#		becomes part of CMake distribution sometimes.

# - Find LIBZIP
# Find the native LIBZIP headers and libraries.
#
# LIBZIP_INCLUDE_DIRS	- where to find LIBZIP.h, etc.
# LIBZIP_LIBRARIES	- List of libraries when using sqlite.
# LIBZIP_FOUND	- True if sqlite found.

# Look for the header file.
FIND_PATH(LIBZIP_INCLUDE_DIR NAMES zip.h)

# Look for the library.
FIND_LIBRARY(LIBZIP_LIBRARY NAMES libzip.a zip)

# Handle the QUIETLY and REQUIRED arguments and set LIBZIP_FOUND to TRUE if all listed variables are TRUE.
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(LIBZIP DEFAULT_MSG LIBZIP_LIBRARY LIBZIP_INCLUDE_DIR)

# Copy the results to the output variables.
IF(LIBZIP_FOUND)
    SET(LIBZIP_LIBRARIES ${LIBZIP_LIBRARY})
    SET(LIBZIP_INCLUDE_DIRS ${LIBZIP_INCLUDE_DIR})
ELSE(LIBZIP_FOUND)
    SET(LIBZIP_LIBRARIES)
    SET(LIBZIP_INCLUDE_DIRS)
ENDIF(LIBZIP_FOUND)

MARK_AS_ADVANCED(LIBZIP_INCLUDE_DIRS LIBZIP_LIBRARIES)