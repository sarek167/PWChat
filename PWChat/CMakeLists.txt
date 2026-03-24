cmake_minimum_required(VERSION 3.19)
project(PWChat LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core)

qt_standard_project_setup()

qt_add_executable(PWChat
    main.cpp
    common/CMakeLists.txt
    client/CMakeLists.txt
    server/CMakeLists.txt
    server/include/server/Server.h
    common/include/common/Packet.h
    common/src/Packet.cpp
    server/src/Server.cpp
    client/include/client/NetworkManager.h
    client/src/NetworkManager.cpp
)

target_link_libraries(PWChat
    PRIVATE
        Qt::Core
)

include(GNUInstallDirs)

install(TARGETS PWChat
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
