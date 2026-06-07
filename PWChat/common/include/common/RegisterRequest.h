#ifndef REGISTERREQUEST_H
#define REGISTERREQUEST_H

#include <string>
#include <cereal/types/string.hpp>
#include <cstdint>

/**
 * @struct RegisterRequest
 * @brief Struct used to send a registration request for a new user account.
 * * It packages the credentials needed by the server to create a new user profile
 * and save it in the database.
 */
struct RegisterRequest {
    uint32_t id;          /**< ID number assigned and filled in by the server after successful registration. */
    std::string nickname; /**< The chosen nickname for the new account. */
    std::string password; /**< The password chosen to secure the new account. */

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

#endif // REGISTERREQUEST_H
