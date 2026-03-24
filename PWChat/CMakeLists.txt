cmake_minimum_required(VERSION 3.19)
project(PWChat LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Network Widgets Multimedia Sql)

add_subdirectory(common)
add_subdirectory(server)
add_subdirectory(client)
