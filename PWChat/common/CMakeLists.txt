add_library(common STATIC
    src/Packet.cpp
    include/common/Packet.h
    include/common/PacketHeader.h
    include/common/MessageType.h
)

target_include_directories(common PUBLIC
    include
    ${CMAKE_SOURCE_DIR}/libs/cereal/include
)
