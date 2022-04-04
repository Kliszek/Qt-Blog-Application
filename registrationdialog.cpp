#include "registrationdialog.h"
#include "ui_registrationdialog.h"
#include "user.h"
#include "usermanager.h"

RegistrationDialog::RegistrationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegistrationDialog)
{
    ui->setupUi(this);
}

RegistrationDialog::~RegistrationDialog()
{
    qInfo() << "Registration deconstructed";
    delete ui;
}

bool RegistrationDialog::validateCredentials()
{
    if(!UserManager::checkAvailability(ui->txtUsername->text(), ui->txtEmail->text(), ui->txtId->text()))
    {
        QMessageBox::critical(this, "Error", "This email or username is already in use!");
        return false;
    }

    if(ui->txtPassword->text() != ui->txtPassword_2->text())
    {
        QMessageBox::critical(this, "Error", "Passwords do not match!");
        return false;
    }
    return true;
}

void RegistrationDialog::on_btnRegister_clicked()
{
    User newUser(true);
    newUser.m_username = ui->txtUsername->text();
    newUser.m_email = ui->txtEmail->text();
    newUser.m_password = ui->txtPassword->text();

    if(!validateCredentials())
    {
        QMessageBox::critical(this, "Error", "Provided data is not correct!");
        return;
    }

    UserManager::getUserList()->append(newUser);
    UserManager::saveUsers();
    this->close();

}

