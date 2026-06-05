cmake_minimum_required(VERSION 3.14)
project(PWChatServer LANGUAGES CXX C)

set(SQLITE_SRC ${CMAKE_SOURCE_DIR}/libs/SQLite/sqlite3.c)
set_source_files_properties(${SQLITE_SRC} PROPERTIES LANGUAGE C)

add_library(server_lib STATIC
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
    include/server/Commands/Command.h
    include/server/Commands/JoinRoomCommand.h
    src/Commands/JoinRoomCommand.cpp
    include/server/Commands/CreateRoomCommand.h
    src/Commands/CreateRoomCommand.cpp
    include/server/Commands/LoginCommand.h
    src/Commands/LoginCommand.cpp
    include/server/Commands/UserMessCommand.h
    src/Commands/UserMessCommand.cpp
    include/server/Commands/RoomMessCommand.h
    src/Commands/RoomMessCommand.cpp
    include/server/DBConnector.h
    include/server/SQLiteConnector.h
    src/SQLiteConnector.cpp
    ${SQLITE_SRC}
    include/server/Commands/RegisterCommand.h
    include/server/Commands/LogoutCommand.h
    src/Commands/LogoutCommand.cpp
    include/server/Commands/RoomInfoCommand.h
    src/Commands/RoomInfoCommand.cpp
    src/Commands/RegisterCommand.cpp
    include/server/Commands/LeaveRoomCommand.h
    src/Commands/LeaveRoomCommand.cpp
    src/Commands/AddAdminCommand.cpp
    include/server/Commands/AddAdminCommand.h
    include/server/Commands/LoadMessagesCommand.h
    src/Commands/LoadMessagesCommand.cpp
    include/server/Commands/LoadAudioCommand.h
    src/Commands/LoadAudioCommand.cpp
    include/server/Commands/GenRoomCodeCommand.h
    src/Commands/GenRoomCodeCommand.cpp
    include/server/Commands/FindUserCommand.h
    src/Commands/FindUserCommand.cpp
)

target_include_directories(server_lib PUBLIC
    include
    ${CMAKE_SOURCE_DIR}/libs/asio-1.36.0/include
    ${CMAKE_SOURCE_DIR}/libs/SQLite
)

target_compile_definitions(server_lib PUBLIC ASIO_STANDALONE)

target_link_libraries(server_lib PUBLIC Qt6::Core Qt6::Sql common)

if(WIN32)
    target_link_libraries(server_lib PUBLIC ws2_32 mswsock)
    target_compile_definitions(server_lib PUBLIC _WIN32_WINNT=0x0A00 NOMINMAX)
else()
    find_package(Threads REQUIRED)
    target_link_libraries(server_lib PUBLIC Threads::Threads)
endif()

add_executable(server src/main.cpp)
target_link_libraries(server PRIVATE server_lib)

set_target_properties(server PROPERTIES WIN32_EXECUTABLE FALSE)
