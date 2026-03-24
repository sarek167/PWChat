add_executable(server
    src/main.cpp
    src/Server.cpp
    src/Session.cpp
)

target_include_directories(server PRIVATE
    include
    ${CMAKE_SOURCE_DIR}/libs/asio-1.36.0/include
)

target_compile_definitions(server PRIVATE ASIO_STANDALONE)

target_link_libraries(server PRIVATE Qt6::Core Qt6::Network Qt6::Sql common)

if(WIN32)
    target_link_libraries(server PRIVATE ws2_32 mswsock)
    target_compile_definitions(server PRIVATE NOMINMAX)
else()
    find_package(Threads REQUIRED)
    target_link_libraries(server PRIVATE Threads::Threads)
endif()


