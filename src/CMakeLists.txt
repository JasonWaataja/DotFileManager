set (SOURCES main.cc module.cc modulecomponent.cc filemodulecomponent.cc
	options.cc)

add_executable (dfm ${SOURCES})

find_package (Boost 1.60.0 COMPONENTS filesystem)
include_directories (${Boost_INCLUDE_DIRS})
target_link_libraries (dfm ${Boost_LIBRARIES})
