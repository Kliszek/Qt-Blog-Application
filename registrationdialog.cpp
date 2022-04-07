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

    ui->txtUsernameErr->setText("Username has to be 4-20 characters long!");
    ui->txtEmailErr->setText("Provided email address is not correct!");
    ui->txtIdErr->setText("Unique Id has to be 4-32 characters long!");
    ui->txtPasswordErr->setText("Password has to be 4-20 characters long!");
    ui->txtPassword_2Err->setText("Passwords do not match!");

    for(QLabel* errLabel : ui->boxRegForm->findChildren<QLabel*>(QString(),Qt::FindDirectChildrenOnly))
        if(errLabel->objectName().contains("Err"))
            errLabel->setVisible(false);

    for(QLineEdit* field : ui->boxRegForm->findChildren<QLineEdit*>())
        if(!field->hasAcceptableInput())
        {
            field->parent()->findChild<QLabel*>(field->objectName()+"Err")->setVisible(true);
            valid = false;
        }

    if(ui->txtPassword->text() != ui->txtPassword_2->text())
    {
        //ui->txtPassword_2->setStyleSheet("border: 1px solid red");
        ui->txtPassword_2Err->setVisible(true);
        valid = false;
    }

    if(!UserManager::idAvailable(ui->txtId->text()))
    {
        //ui->txtId->setStyleSheet("border: 1px solid red");
        ui->txtIdErr->setText("This Id is already taken!");
        ui->txtIdErr->setVisible(true);
        valid = false;
    }
    if(!UserManager::usernameAvailable(ui->txtUsername->text()))
    {
        //ui->txtUsername->setStyleSheet("border: 1px solid red");
        ui->txtUsernameErr->setText("This username is already taken!");
        ui->txtUsernameErr->setVisible(true);
        valid = false;
    }
    if(!UserManager::emailAvailable(ui->txtEmail->text()))
    {
        //ui->txtEmail->setStyleSheet("border: 1px solid red");
        ui->txtEmailErr->setText("This email is already being used!");
        ui->txtEmailErr->setVisible(true);
        valid = false;
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

void RegistrationDialog::trimInputs()
{
    ui->txtEmail->setText(ui->txtEmail->text().trimmed());
    ui->txtId->setText(ui->txtId->text().trimmed());
    ui->txtUsername->setText(ui->txtUsername->text().trimmed());
}

void RegistrationDialog::on_btnRegister_clicked()
{
    trimInputs();

    if(!validateCredentials())
    {
        //QMessageBox::critical(this, "Error", "Provided data is not correct!");
        return;
    }

    QString id = ui->txtId->text();
    QString username = ui->txtUsername->text();
    QString email = ui->txtEmail->text();
    QString password = ui->txtPassword->text();

    User newUser(id, email, username, password);

    UserManager::getUserList()->append(newUser);
    if(UserManager::saveUsers())
        QMessageBox::information(this, "Success!", "Account successfully created!");
    else
        QMessageBox::critical(this, "Error", "Account is saved to the memory, but will be lost when the application is closed!");
    this->close();

}


void RegistrationDialog::on_chkAutoId_toggled(bool checked)
{
    if(checked)
    {
        ui->txtId->setEnabled(false);
        ui->txtId->setText(QUuid::createUuid().toString(QUuid::Id128));
        //ui->txtId->setStyleSheet("");
        ui->txtIdErr->setVisible(false);
    }
    else
    {
        ui->txtId->setEnabled(true);
        ui->txtId->clear();
    }
}

