cmake_minimum_required(VERSION 3.19)
project(chatServer LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core LinguistTools)

qt_standard_project_setup()

qt_add_executable(chatServer
    main.cpp
)

qt_add_translations(
    TARGETS chatServer
    TS_FILES chatServer_en_VG.ts
)

target_link_libraries(chatServer
    PRIVATE
        Qt::Core
)

include(GNUInstallDirs)

install(TARGETS chatServer
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
