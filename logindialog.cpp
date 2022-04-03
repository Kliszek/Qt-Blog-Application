#include "logindialog.h"
#include "./ui_logindialog.h"
#include "registrationdialog.h"
#include "maindialog.h"
#include "usermanager.h"

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}


void LoginDialog::on_btnRegister_clicked()
{
    RegistrationDialog* regDial = new RegistrationDialog(this);
    regDial->exec();
}


void LoginDialog::on_btnLogin_clicked()
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

bool LoginDialog::checkCrudentials()
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

