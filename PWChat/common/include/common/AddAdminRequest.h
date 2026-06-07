#ifndef ADDADMINREQUEST_H
#define ADDADMINREQUEST_H

#include <stdint.h>

/**
 * @struct AddAdminRequest
 * @brief Data transfer object (DTO) struct used for requesting to promote a user to an admin.
 * * This structure holds the identifiers necessary to grant moderator or administrative
 * privileges to a specific user inside a specific chat room channel.
 */
struct AddAdminRequest {
    uint32_t roomId; /**< Unique identifier of the target chat room. */
    uint32_t userId; /**< Unique identifier of the user to be promoted. */

    /**
     * @brief Template method used by Cereal to pack or unpack data.
     * @tparam Archive Type of the archive (e.g., binary, XML, JSON).
     * @param archive Reference to the archive object performing the input or output operation.
     */
    template<class Archive>
    void serialize(Archive & archive) {
        archive(roomId, userId);
    }
};

#endif // ADDADMINREQUEST_H
