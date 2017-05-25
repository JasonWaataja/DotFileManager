set (SOURCES main.cc dotfilemanager.cc)

include_directories (${CMAKE_SOURCE_DIR}/DotFileManagerCommon/src)

add_executable (dfm ${SOURCES})
add_dependencies (dfm dfmcommon)
target_link_libraries (dfm dfmcommon)
set_property (TARGET dfm PROPERTY CXX_STANDARD 11)
install (TARGETS dfm DESTINATION bin)
