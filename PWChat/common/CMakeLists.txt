add_library(common STATIC
    src/Packet.cpp
    include/common/Packet.h
    include/common/PacketHeader.h
    include/common/MessageType.h
    include/common/User.h
    src/User.cpp
    include/common/CreateRoomRequest.h
    include/common/LoginRequest.h
    include/common/JoinRoomRequest.h
    include/common/RoomData.h
)

target_include_directories(common PUBLIC
    include
    ${CMAKE_SOURCE_DIR}/libs/cereal/include
)
