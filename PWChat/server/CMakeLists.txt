add_executable(server
    src/main.cpp
    src/Server.cpp
    src/Session.cpp
    include/server/Room.h
    include/server/PublicRoom.h
    include/server/PrivateRoom.h
    src/PublicRoom.cpp
    src/PrivateRoom.cpp
    src/Room.cpp
    include/server/RoomManager.h
    src/RoomManager.cpp
    include/server/Command.h
    include/server/JoinRoomCommand.h
    src/JoinRoomCommand.cpp
    include/server/CreateRoomCommand.h
    src/CreateRoomCommand.cpp
    include/server/LoginCommand.h
    src/LoginCommand.cpp
    include/server/UserTextMessCommand.h
    src/UserTextMessCommand.cpp
    include/server/RoomTextMessCommand.h
    src/RoomTextMessCommand.cpp
)

set_target_properties(server PROPERTIES WIN32_EXECUTABLE FALSE)

target_include_directories(server PRIVATE
    include
    ${CMAKE_SOURCE_DIR}/libs/asio-1.36.0/include
)

target_compile_definitions(server PRIVATE ASIO_STANDALONE)

target_link_libraries(server PRIVATE Qt6::Core Qt6::Network Qt6::Sql common)

if(WIN32)
    target_link_libraries(server PRIVATE ws2_32 mswsock)
    target_compile_definitions(server PRIVATE _WIN32_WINNT=0x0A00 NOMINMAX)
else()
    find_package(Threads REQUIRED)
    target_link_libraries(server PRIVATE Threads::Threads)
endif()
