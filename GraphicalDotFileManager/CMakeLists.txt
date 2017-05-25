project (GraphicalDotFileManager)

add_subdirectory (src)

file (COPY "gdfm.desktop" DESTINATION ${CMAKE_BINARY_DIR})
find_program (XDG-DESKTOP-MENU_EXECUTABLE xdg-desktop-menu)
install (CODE "execute_process(COMMAND ${XDG-DESKTOP-MENU_EXECUTABLE} install --novendor gdfm.desktop)")
