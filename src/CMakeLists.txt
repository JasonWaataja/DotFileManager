set (SOURCES main.cc module.cc moduleaction.cc installaction.cc removeaction.cc
	options.cc shellaction.cc messageaction.cc configfilereader.cc command.cc
	dependencyaction.cc)

add_executable (dfm ${SOURCES})
set_property(TARGET dfm PROPERTY CXX_STANDARD 11)

find_package (Boost 1.55.0 COMPONENTS filesystem regex)
include_directories (${Boost_INCLUDE_DIRS})
target_link_libraries (dfm ${Boost_LIBRARIES})

# Installing
install (TARGETS dfm DESTINATION bin)
