#ifndef LOGINREQUEST_H
#define LOGINREQUEST_H

#include <string>
#include <cereal/types/string.hpp>
#include <cstdint>

/**
 * @struct LoginRequest
 * @brief Data transfer object (DTO) struct used for transmitting user login credentials.
 * * This structure carries the primary parameters required by the server backend
 * to verify identity credentials and authorize a client session connection.
 */
struct LoginRequest {
    uint32_t id;             /**< Unique numerical identification index tracking key (if known/assigned). */
    std::string nickname;    /**< The distinct display handle or nickname credentials entered by the user. */
    std::string password;    /**< The security password string payload required to validate authentication. */

    /**
     * @brief Template method used by Cereal to pack or unpack data.
     * @tparam Archive Type of the archive (e.g., binary, XML, JSON).
     * @param archive Reference to the archive object performing the input or output operation.
     */
    template<class Archive>
    void serialize(Archive & archive) {
        archive(id, nickname, password);
    }
};

#endif // LOGINREQUEST_H
