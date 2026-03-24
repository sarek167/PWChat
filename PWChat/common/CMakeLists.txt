add_library(common STATIC
    src/Packet.cpp
    include/common/Packet.h
)

target_include_directories(common PUBLIC include)
