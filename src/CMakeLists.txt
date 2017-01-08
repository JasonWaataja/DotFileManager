include (CheckIncludeFiles)

set (SOURCES main.cc module.cc moduleaction.cc installaction.cc removeaction.cc
	options.cc shellaction.cc messageaction.cc configfilereader.cc command.cc
	dependencyaction.cc readerenvironment.cc filecheckaction.cc
	dotfilemanager.cc util.cc)

check_include_files (wordexp.h HAVE_WORDEXP_H)
configure_file (${CMAKE_CURRENT_SOURCE_DIR}/config.h.in
	${CMAKE_CURRENT_BINARY_DIR}/config.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable (dfm ${SOURCES})
set_property(TARGET dfm PROPERTY CXX_STANDARD 11)
install (TARGETS dfm DESTINATION bin)
