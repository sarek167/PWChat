set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

add_executable(client WIN32
    src/main.cpp
    src/NetworkManager.cpp
)

target_include_directories(client PRIVATE
    include
    ${CMAKE_SOURCE_DIR}/libs/asio-1.36.0/include
)

target_compile_definitions(client PRIVATE ASIO_STANDALONE)

target_include_directories(client PRIVATE include)
target_link_libraries(client PRIVATE Qt6::Widgets Qt6::Network Qt6::Multimedia common)
