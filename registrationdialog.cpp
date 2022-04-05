#include "registrationdialog.h"
#include "ui_registrationdialog.h"
#include "user.h"
#include "usermanager.h"

RegistrationDialog::RegistrationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegistrationDialog)
{
    ui->setupUi(this);
    setValidators();
}

RegistrationDialog::~RegistrationDialog()
{
    qInfo() << "Registration deconstructed";
    delete ui;
}

bool RegistrationDialog::validateCredentials()
{
    bool valid = true;

    for(QLineEdit* field : ui->boxRegForm->findChildren<QLineEdit*>())
    {
        if(!field->hasAcceptableInput())
        {
            field->setStyleSheet("QLineEdit {border: 1px solid red;}");
            valid = false;
        }
        else
        {
            field->setStyleSheet("");
        }
    }

    if(!UserManager::checkAvailability(ui->txtUsername->text(), ui->txtEmail->text(), ui->txtId->text()))
    {
        QMessageBox::critical(this, "Error", "This email or username is already in use!");
        ui->txtUsername->setStyleSheet("QLineEdit {border: 1px solid red;}");
        return false;
    }

    if(ui->txtPassword->text() != ui->txtPassword_2->text())
    {
        QMessageBox::critical(this, "Error", "Passwords do not match!");
        ui->txtPassword_2->setStyleSheet("QLineEdit {border: 1px solid red;}");
        return false;
    }
    return valid;
}

void RegistrationDialog::setValidators()
{
    QRegularExpression rxNormal("^[a-zA-Z0-9 _\\-]{4,20}$");
    QRegularExpression rxId("^[a-zA-Z0-9]{4,32}$");
    //QRegularExpression rxEmail("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,4}$");
    QRegularExpression rxEmail("^[a-zA-Z0-9_.+-]+@[a-zA-Z0-9-]+.[a-zA-Z0-9-.]+$");
    ui->txtId->setValidator(new QRegularExpressionValidator(rxId, this));
    ui->txtPassword->setValidator(new QRegularExpressionValidator(rxNormal, this));
    ui->txtUsername->setValidator(new QRegularExpressionValidator(rxNormal, this));
    ui->txtEmail->setValidator(new QRegularExpressionValidator(rxEmail, this));
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

