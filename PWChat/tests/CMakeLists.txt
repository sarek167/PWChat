cmake_minimum_required(VERSION 3.14)
project(AppUnitTests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Test)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(run_unit_tests
    ${CMAKE_CURRENT_SOURCE_DIR}/common/PacketTest.cpp
    server/SQLiteConnectorTest.cpp
    server/ServerTest.cpp
    server/RoomManagerTest.cpp
    client/OpusCodecTest.cpp
    client/AudioManagerTest.cpp
    client/NetworkManagerTest.cpp

)

target_include_directories(run_unit_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)

target_link_libraries(run_unit_tests PRIVATE
    gtest_main
    gtest
    gmock
    Qt6::Core
    Qt6::Test
    common
    server_lib
    client_lib
)

add_test(NAME AllUnitTests COMMAND run_unit_tests)
