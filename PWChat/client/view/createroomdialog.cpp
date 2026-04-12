#include "createroomdialog.h"
#include "ui_createroomdialog.h"

CreateRoomDialog::CreateRoomDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CreateRoomDialog)
{
    ui->setupUi(this);
}

CreateRoomDialog::~CreateRoomDialog()
{
    delete ui;
}

void CreateRoomDialog::on_buttonBox_accepted()
{
    std::string roomName = ui->editName->text().toStdString();
    bool isPrivate = ui->btnPrivate->isChecked();
    bool isAdmin = ui->btnAdmin->isChecked();

    emit createRoomRequested(roomName, isPrivate, isAdmin);
}

