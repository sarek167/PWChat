include(FetchContent)

FetchContent_Declare(
  opus
  GIT_REPOSITORY https://github.com/xiph/opus.git
  GIT_TAG        v1.4
)

set(OPUS_BUILD_PROGRAMS OFF CACHE BOOL "" FORCE)
set(OPUS_BUILD_TESTING OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(opus)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/ui")

add_executable(client WIN32
    src/main.cpp
    src/NetworkManager.cpp
    include/client/NetworkManager.h
    ui/LoginWindow.ui
    ui/mainwindow.ui
    view/loginwindow.h
    view/loginwindow.cpp
    view/mainwindow.h
    view/mainwindow.cpp

    resources/resources.qrc
    view/createroomdialog.h
    view/createroomdialog.cpp
    ui/createroomdialog.ui
    include/client/AppManager.h
    src/AppManager.cpp
    include/client/AudioManager.h
    src/AudioManager.cpp
    include/client/AudioCodec.h
    include/client/OpusCodec.h
    src/OpusCodec.cpp
)

target_include_directories(client PRIVATE
    include
    view
    ${CMAKE_SOURCE_DIR}/libs/asio-1.36.0/include
)

target_compile_definitions(client PRIVATE ASIO_STANDALONE)

target_include_directories(client PRIVATE include)
target_link_libraries(client PRIVATE Qt6::Widgets Qt6::Network Qt6::Multimedia opus common)
