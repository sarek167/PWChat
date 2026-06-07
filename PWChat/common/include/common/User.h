#ifndef USER_H
#define USER_H

#include <string>
#include <stdint.h>

/**
 * @class User
 * @brief Class that represents a user object in the application.
 * * This class keeps track of basic user properties like their ID
 * and nickname, providing standard getters and setters to change them.
 */
class User {
public:
    /**
     * @brief Constructor for User class.
     * @param id Unique ID number assigned to the user.
     * @param nickname The chosen nickname of the user.
     */
    User(uint32_t id, std::string nickname);

    /**
     * @brief Destructor for User class.
     */
    ~User() = default;

    /**
     * @brief Getter for the user's ID.
     * @return const uint32_t The ID number of the user.
     */
    const uint32_t id() const;

    /**
     * @brief Getter for the user's nickname.
     * @return const std::string The user's nickname string.
     */
    const std::string nickname() const;

    /**
     * @brief Setter to update the user's ID.
     * @param id The new ID number to assign.
     */
    void setId(uint32_t id);

    /**
     * @brief Setter to update the user's nickname.
     * @param nick The new nickname string to set.
     */
    void setNickname(std::string nick);

private:
    uint32_t m_id;            /**< Internal variable holding the user's ID. */
    std::string m_nickname;   /**< Internal variable holding the user's nickname string. */
};

#endif // USER_H
