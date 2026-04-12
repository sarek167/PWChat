#ifndef CREATEROOMDIALOG_H
#define CREATEROOMDIALOG_H

#include <QDialog>

namespace Ui {
class CreateRoomDialog;
}

class CreateRoomDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateRoomDialog(QWidget *parent = nullptr);
    ~CreateRoomDialog();

signals:
    void createRoomRequested(std::string roomName, bool isPrivate, bool isAdmin);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::CreateRoomDialog *ui;
};

#endif // CREATEROOMDIALOG_H
