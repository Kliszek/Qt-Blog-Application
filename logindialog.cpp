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
    tryLogIn();
}

bool LoginDialog::tryLogIn()
{
    QString password = UserManager::getPassword(ui->txtUsername->text());

    const User* user = UserManager::getUser(ui->txtUsername->text());

    if(user == nullptr)
    {
        QMessageBox::critical(this, "Error", "Provided username does not belong to any user!");
        return false;
    }

    if(password != ui->txtPassword->text())
    {
        QMessageBox::critical(this, "Error", "Provided password is not correct!");
        return false;
    }

    MainDialog* mainDial = new MainDialog(user);

    this->close();
    mainDial->show();

    return true;
}

