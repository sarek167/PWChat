#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    uint32_t m_targetId;
    std::string m_message;
    bool m_toRoom;

signals:
    void sendRequested(uint32_t targetId, std::string message, bool toRoom);

private slots:
    void on_btnSend_clicked();
};

#endif // MAINWINDOW_H
