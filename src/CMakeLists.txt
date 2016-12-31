set (SOURCES main.cc module.cc moduleaction.cc installaction.cc removeaction.cc
	options.cc shellaction.cc messageaction.cc configfilereader.cc command.cc
	dependencyaction.cc readerenvironment.cc filecheckaction.cc
	dotfilemanager.cc util.cc)

add_executable (dfm ${SOURCES})
set_property(TARGET dfm PROPERTY CXX_STANDARD 11)
install (TARGETS dfm DESTINATION bin)
