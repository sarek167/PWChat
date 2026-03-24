set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

add_executable(client
    src/main.cpp
    src/NetworkManager.cpp
)

target_include_directories(client PRIVATE include)
target_link_libraries(client PRIVATE Qt6::Widgets Qt6::Network Qt6::Multimedia common)
