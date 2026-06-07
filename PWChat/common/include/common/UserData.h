#ifndef USERDATA_H
#define USERDATA_H

#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>
#include <stdint.h>
#include <string>

/**
 * @struct UserData
 * @brief Data transfer object (DTO) struct containing basic public profile metadata of a user.
 * * This lightweight structure is used across various network packets to exchange minimalist
 * identification parameters (like ID and nickname pairs) without passing sensitive session details.
 */
struct UserData {
    uint32_t id;             /**< Unique numerical identifier assigned to the user. */
    std::string nickname;    /**< The distinct public display handle or nickname of the user. */

    /**
     * @brief Template method used by Cereal to pack or unpack data.
     * @tparam Archive Type of the archive (e.g., binary, XML, JSON).
     * @param archive Reference to the archive object performing the input or output operation.
     */
    template<class Archive>
    void serialize(Archive & archive) {
        archive(id, nickname);
    }
};

#endif // USERDATA_H
