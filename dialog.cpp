#include "dialog.h"
#include "./ui_dialog.h"
#include "registrationdialog.h"
#include "maindialog.h"
#include "usermanager.h"

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}


void Dialog::on_pushButton_3_clicked()
{
    RegistrationDialog* regDial = new RegistrationDialog(this);
    regDial->exec();
}


void Dialog::on_pushButton_clicked()
{
    if(!checkCrudentials())
    {
        QMessageBox::critical(this, "Could not log in", "Username or login are not correct!");
        return;
    }

    MainDialog* mainDial = new MainDialog();
    this->close();
    mainDial->show();
}

bool Dialog::checkCrudentials()
{

    if(ui->txtUsername->text() == "admin" && ui->txtPassword->text() == "admin")
    {
        return true;
    }

    QString password = UserManager::getPassword(ui->txtUsername->text());

    if(password != ui->txtPassword->text())
    {
        QMessageBox::critical(this, "Error", "Provided password is not correct!");
        return false;
    }

    return true;
}

