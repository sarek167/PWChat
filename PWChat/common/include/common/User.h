#ifndef USER_H
#define USER_H

#include <string>
#include <stdint.h>

/**
 * @class User
 * @brief Class representing a user entity within the local application session context.
 * * This class manages the local state properties of a user, such as their unique
 * identification index and display nickname handle, providing standard accessors and mutators.
 */
class User {
public:
    /**
     * @brief Constructor for User class.
     * @param id Unique numerical identifier assigned to the user.
     * @param nickname The distinct display name or handle of the user.
     */
    User(uint32_t id, std::string nickname);

    /**
     * @brief Destructor for User class.
     */
    ~User() = default;

    /**
     * @brief Getter for the user's unique identifier.
     * @return const uint32_t The unique numerical ID tracking key.
     */
    const uint32_t id() const;

    /**
     * @brief Getter for the user's display nickname handle.
     * @return const std::string The string containing the user's nickname.
     */
    const std::string nickname() const;

    /**
     * @brief Setter for updating the user's unique identifier.
     * @param id The new unique numerical identifier to assign.
     */
    void setId(uint32_t id);

    /**
     * @brief Setter for updating the user's display nickname handle.
     * @param nick The new nickname string to set.
     */
    void setNickname(std::string nick);

private:
    uint32_t m_id;            /**< Internal tracking key representing the unique identification index. */
    std::string m_nickname;   /**< Internal string caching the distinct display handle or nickname credentials. */
};

#endif // USER_H
