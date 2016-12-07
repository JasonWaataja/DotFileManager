set (SOURCES main.cc)

add_executable (dfm ${SOURCES})

find_package (Boost 1.60.0 REQUIRED)
include_directories (${Boost_INCLUDE_DIRS})
target_link_libraries (dfm ${Boost_LIBRARIES})
