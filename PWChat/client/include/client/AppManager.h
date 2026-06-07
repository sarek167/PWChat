#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H

#include <QObject>
#include <QPushButton>
#include "client/NetworkManager.h"
#include "loginwindow.h"
#include "mainwindow.h"
#include "client/AudioManager.h"

/**
 * @class AppManager
 * @brief Class responsible for client side connection between all app segments while keeping them separate.
 * * It acts as the core orchestrator (controller) of the client application, managing the lifecycle
 * of user interface windows and binding business logic from network and audio systems together.
 */
class AppManager : public QObject {
    Q_OBJECT
public:
    /**
     * @brief Constructor for AppManager class.
     * Initializes all internal subsystems including NetworkManager and AudioManager.
     * @param parent Pointer to the parent QObject (defaults to nullptr).
     */
    explicit AppManager(QObject *parent=nullptr);

    /**
     * @brief Destructor for AppManager class.
     */
    ~AppManager() = default;

    /**
     * @brief Connects client to the server and shows login window.
     */
    void start();

private:
    NetworkManager* m_networkManager;               /**< Pointer to the NetworkManager class instance */
    AudioManager* m_audioManager;                   /**< Pointer to the AudioManager class instance */
    LoginWindow m_loginWin;                         /**< LoginWindow object to show to user */
    MainWindow m_mainWin;                           /**< MainWindow object to show to user */
    QPushButton* m_currentPlayingButton = nullptr;  /**< Pointer to the currently playing audio message button */

    /**
     * @brief Responsible for setting up all connections and transferring signals from different app elements.
     * * Connects signals from UI windows to appropriate slots in NetworkManager/AudioManager
     * and routes network response events back to UI display methods.
     */
    void setupConnections();
};

#endif // APPCONTROLLER_H
