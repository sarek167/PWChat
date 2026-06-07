#ifndef CREATEROOMDIALOG_H
#define CREATEROOMDIALOG_H

#include <QDialog>

namespace Ui {
class CreateRoomDialog;
}

/**
 * @class CreateRoomDialog
 * @brief Class representing widget for creating room.
 */
class CreateRoomDialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Constructor for class CreateRoomDialog.
     * @param parent Pointer to the parent widget (defaults to nullptr).
     */
    explicit CreateRoomDialog(QWidget *parent = nullptr);

    /**
     * @brief Destructor for class CreateRoomDialog.
     */
    ~CreateRoomDialog();

signals:
    /**
     * @brief Signal which is sent when user filled in all information and clicks submit button.
     * It is forwarded to NetworkManager to send to server.
     * @param roomName The unique textual name or title designed for the new room channel.
     * @param isPrivate Boolean flag specifying if the channel requires an access code verification (true) or is public (false).
     * @param isAdmin Boolean flag defining if the room creator should be assigned administrative operator rights.
     */
    void createRoomRequested(std::string roomName, bool isPrivate, bool isAdmin);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::CreateRoomDialog *ui; /**< Pointer to CreateRoomDialog ui. */
};

#endif // CREATEROOMDIALOG_H
