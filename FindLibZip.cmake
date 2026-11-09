# Finds the libzip headers and library.
#
# The expected include convention is
#   #include <zip.h>
# i.e. LibZip_INCLUDE_DIR is the directory that directly contains zip.h.
#
# Result variables:
#   LibZip_FOUND         - True if libzip was found.
#   LibZip_INCLUDE_DIRS  - Directory containing zip.h.
#   LibZip_LIBRARIES     - Library to link against.
#
# Imported target:
#   LibZip::LibZip
#
# Hints:
#   Set LIBZIP_DIR (CMake variable or environment variable) to the prefix of a
#   libzip installation, e.g. -DLIBZIP_DIR=/usr/local

find_path(
    LibZip_INCLUDE_DIRS NAMES zip.h
    HINTS ${LIBZIP_DIR} $ENV{LIBZIP_DIR}
    PATH_SUFFIXES include
)

find_library(
    LibZip_LIBRARIES NAMES libzip zip
    HINTS ${LIBZIP_DIR} $ENV{LIBZIP_DIR}
    PATH_SUFFIXES lib lib64
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LibZip
    REQUIRED_VARS LibZip_LIBRARIES LibZip_INCLUDE_DIRS
    FAIL_MESSAGE "libzip not found. Install the libzip development package or set LIBZIP_DIR to its prefix.")

if(LibZip_FOUND)
    add_library(LibZip::LibZip UNKNOWN IMPORTED)
    set_target_properties(LibZip::LibZip PROPERTIES
        IMPORTED_LOCATION "${LibZip_LIBRARIES}"
        INTERFACE_INCLUDE_DIRECTORIES "${LibZip_INCLUDE_DIRS}")
endif()

mark_as_advanced(LibZip_INCLUDE_DIR LibZip_LIBRARY)