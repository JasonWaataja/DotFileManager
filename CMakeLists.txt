cmake_minimum_required (VERSION 2.9)
project(DotFileManager)

if (NOT ${UNIX})
	message (FATAL_ERROR "Can only compile on UNIX platform, exiting.")
endif (NOT ${UNIX})

set (CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/Modules ${CMAKE_MODULE_PATH})

add_subdirectory (src)
add_subdirectory (man)
