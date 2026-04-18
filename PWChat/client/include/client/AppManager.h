#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H
#include <QObject>
#include "client/NetworkManager.h"
#include "loginwindow.h"
#include "mainwindow.h"
#include "client/AudioManager.h"

class AppManager : public QObject {
    Q_OBJECT
public:
    explicit AppManager(QObject *parent=nullptr);
    void start();
private:
    NetworkManager* m_networkManager;
    AudioManager* m_audioManager;
    LoginWindow m_loginWin;
    MainWindow m_mainWin;

    void setupConnections();
};

#endif // APPCONTROLLER_H
