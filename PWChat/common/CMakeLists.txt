add_library(common STATIC
    src/Packet.cpp
    include/common/Packet.h
    include/common/PacketHeader.h
)

target_include_directories(common PUBLIC include)
