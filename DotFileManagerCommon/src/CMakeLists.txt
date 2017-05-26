include (CheckIncludeFiles)

set (SOURCES module.cc moduleaction.cc installaction.cc removeaction.cc
	options.cc shellaction.cc messageaction.cc configfilereader.cc command.cc
	dependencyaction.cc readerenvironment.cc filecheckaction.cc util.cc
	configfilewriter.cc modulefile.cc)

if (HAS_GRAPHICS)
	include_directories (${CMAKE_SOURCE_DIR}/GraphicalDotFileManager/src)
endif (HAS_GRAPHICS)

check_include_files (wordexp.h HAVE_WORDEXP_H)
configure_file (${CMAKE_CURRENT_SOURCE_DIR}/config.h.in
	${CMAKE_CURRENT_BINARY_DIR}/config.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR})


add_library (dfmcommon STATIC ${SOURCES})
if (HAS_GRAPHICS)
	find_package (PkgConfig REQUIRED)
	pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)
	include_directories(${GTKMM_INCLUDE_DIRS})
	link_directories(${GTKMM_LIBRARY_DIRS})
	add_definitions(${GTKMM_CFLAGS_OTHER})
	target_link_libraries(dfmcommon ${GTKMM_LIBRARIES})
endif (HAS_GRAPHICS)
set_property(TARGET dfmcommon PROPERTY CXX_STANDARD 11)
