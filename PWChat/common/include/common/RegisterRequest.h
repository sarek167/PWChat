#ifndef REGISTERREQUEST_H
#define REGISTERREQUEST_H

#include <string>
#include <cereal/types/string.hpp>
#include <cstdint>

/**
 * @struct RegisterRequest
 * @brief Data transfer object (DTO) struct used for submitting a new user registration request.
 * * This structure carries the desired credentials required by the server backend
 * to allocate a new identity profile, store it in the database, and assign a unique user ID.
 */
struct RegisterRequest {
    uint32_t id;          /**< Unique numerical identifier tracking key, populated by the server upon successful creation. */
    std::string nickname; /**< The chosen unique username or display handle handle requested for the new account. */
    std::string password; /**< The security password string payload chosen to secure the new account profile. */

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

#endif // REGISTERREQUEST_H
