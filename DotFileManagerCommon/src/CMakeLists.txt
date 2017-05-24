include (CheckIncludeFiles)

set (SOURCES module.cc moduleaction.cc installaction.cc removeaction.cc
	options.cc shellaction.cc messageaction.cc configfilereader.cc command.cc
	dependencyaction.cc readerenvironment.cc filecheckaction.cc util.cc
	configfilewriter.cc modulefile.cc)

check_include_files (wordexp.h HAVE_WORDEXP_H)
configure_file (${CMAKE_CURRENT_SOURCE_DIR}/config.h.in
	${CMAKE_CURRENT_BINARY_DIR}/config.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library (dfmcommon STATIC ${SOURCES})
set_property(TARGET dfmcommon PROPERTY CXX_STANDARD 11)
