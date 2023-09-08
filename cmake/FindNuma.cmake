# Module for locating libnuma
#
# Read-only variables:
#   Numa_FOUND
#     Indicates that the library has been found.
#
#   Numa_INCLUDE_DIR
#     Points to the libnuma include directory.
#
#   Numa_LIBRARY_DIR
#     Points to the directory that contains the libraries.
#     The content of this variable can be passed to link_directories.
#
#   Numa_LIBRARY
#     Points to the libnuma that can be passed to target_link_libararies.
#
# Copyright (c) 2013-2017 MulticoreWare, Inc

include(FindPackageHandleStandardArgs)

find_path(Numa_ROOT_DIR
  NAMES include/numa.h
  PATHS ENV Numa_ROOT
  DOC "Numa root directory")

find_path(Numa_INCLUDE_DIR
  NAMES numa.h
  HINTS ${Numa_ROOT_DIR}
  PATH_SUFFIXES include
  DOC "Numa include directory")

find_library(Numa_LIBRARY
  NAMES numa
  HINTS ${Numa_ROOT_DIR}
  DOC "Numa library")

if (Numa_LIBRARY)
    get_filename_component(Numa_LIBRARY_DIR ${Numa_LIBRARY} PATH)
endif()

mark_as_advanced(Numa_INCLUDE_DIR Numa_LIBRARY_DIR Numa_LIBRARY)

find_package_handle_standard_args(Numa REQUIRED_VARS Numa_ROOT_DIR Numa_INCLUDE_DIR Numa_LIBRARY)
