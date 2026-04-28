set(SQLITE_SRC ${CMAKE_SOURCE_DIR}/libs/SQLite/sqlite3.c)
set_source_files_properties(${SQLITE_SRC} PROPERTIES LANGUAGE C)

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
    include/server/Commands/Command.h
    include/server/Commands/JoinRoomCommand.h
    src/Commands/JoinRoomCommand.cpp
    include/server/Commands/CreateRoomCommand.h
    src/Commands/CreateRoomCommand.cpp
    include/server/Commands/LoginCommand.h
    src/Commands/LoginCommand.cpp
    include/server/Commands/UserTextMessCommand.h
    src/Commands/UserTextMessCommand.cpp
    include/server/Commands/RoomTextMessCommand.h
    src/Commands/RoomTextMessCommand.cpp
    include/server/DBConnector.h
    include/server/DBConnector.h
    include/server/SQLiteConnector.h
    src/SQLiteConnector.cpp
    ${SQLITE_SRC}
    include/server/Commands/UserAudioMessCommand.h
    src/Commands/UserAudioMessCommand.cpp
    include/server/Commands/RegisterCommand.h
    include/server/Commands/LogoutCommand.h
    src/Commands/LogoutCommand.cpp
    include/server/Commands/RoomInfoCommand.h
    src/Commands/RoomInfoCommand.cpp
    src/Commands/RegisterCommand.cpp
)


set_target_properties(server PROPERTIES WIN32_EXECUTABLE FALSE)

target_include_directories(server PRIVATE
    include
    ${CMAKE_SOURCE_DIR}/libs/asio-1.36.0/include
    ${CMAKE_SOURCE_DIR}/libs/SQLite
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
