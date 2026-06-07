#ifndef LOGINREQUEST_H
#define LOGINREQUEST_H

#include <string>
#include <cereal/types/string.hpp>
#include <cstdint>

/**
 * @struct LoginRequest
 * @brief Struct used to send user login credentials to the server.
 * * It packages the nickname and password needed by the server to verify
 * the user's identity and log them into the application.
 */
struct LoginRequest {
    uint32_t id;             /**< User ID number (if already known or assigned). */
    std::string nickname;    /**< The nickname typed in by the user. */
    std::string password;    /**< The password typed in by the user to log in. */

    /**
     * @brief Serialization method used by the Cereal library to pack or unpack this struct.
     * @tparam Archive Type of the data stream (like binary or JSON).
     * @param archive Reference to the archive object that saves or loads the data fields.
     */
    template<class Archive>
    void serialize(Archive & archive) {
        archive(id, nickname, password);
    }
};

#endif // LOGINREQUEST_H
