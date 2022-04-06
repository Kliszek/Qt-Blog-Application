#include "logindialog.h"
#include "./ui_logindialog.h"
#include "registrationdialog.h"
#include "maindialog.h"
#include "usermanager.h"
#include <QCoreApplication>
#include <QThread>
#include <QGuiApplication>

LoginDialog::LoginDialog(const User *loggedUser, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
{
    ui->setupUi(this);

    if(loggedUser == nullptr)
    {
        this->show();
    }
    else
    {
        MainDialog* mainDial = new MainDialog(loggedUser, this);
        mainDial->show();
        mainDial->setAttribute(Qt::WA_DeleteOnClose);
    }
}

LoginDialog::~LoginDialog()
{
    delete ui;
    qInfo() << "Login deconstructed";
}


void LoginDialog::on_btnRegister_clicked()
{
    RegistrationDialog regDial(this);
    regDial.exec();
}


void LoginDialog::on_btnLogin_clicked()
{
    tryLogin();
}



bool LoginDialog::tryLogin()
{
    ui->txtUsernameErr->setText("Provided username does not belong to any user!");
    ui->txtUsernameErr->setVisible(false);
    ui->txtPasswordErr->setText("Provided password is not correct!");
    ui->txtPasswordErr->setVisible(false);

    QString password = UserManager::getPassword(ui->txtUsername->text());

    const User* user = UserManager::getUserByName(ui->txtUsername->text());

    if(user == nullptr)
    {
        //QMessageBox::critical(this, "Error", "Provided username does not belong to any user!");
        ui->txtUsernameErr->setVisible(true);
        return false;
    }

    if(password != ui->txtPassword->text())
    {
        //QMessageBox::critical(this, "Error", "Provided password is not correct!");
        ui->txtPasswordErr->setVisible(true);
        return false;
    }

    if(ui->chkKeepLogged->isChecked())
    {
        QSettings settings(QSettings::UserScope);
        settings.clear();
        settings.setValue("logged", user->getId());
    }

    MainDialog* mainDial = new MainDialog(user, this);

    this->close();

    mainDial->show();
    mainDial->setAttribute(Qt::WA_DeleteOnClose);

    return true;
}

