#ifndef USERDATA_H
#define USERDATA_H

#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>
#include <stdint.h>
#include <string>

/**
 * @struct UserData
 * @brief Simple struct used to hold basic user info like ID and nickname.
 * * It is used as a lightweight structure to quickly send user details
 * over the network without mixing in any sensitive session data.
 */
struct UserData {
    uint32_t id;             /**< Unique ID number assigned to the user. */
    std::string nickname;    /**< The user's visible nickname or handle. */

    /**
     * @brief Serialization method used by the Cereal library to pack or unpack this struct.
     * @tparam Archive Type of the data stream (like binary or JSON).
     * @param archive Reference to the archive object that saves or loads the data fields.
     */
    template<class Archive>
    void serialize(Archive & archive) {
        archive(id, nickname);
    }
};

#endif // USERDATA_H
