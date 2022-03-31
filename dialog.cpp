#include "dialog.h"
#include "./ui_dialog.h"
#include "registrationdialog.h"
#include "maindialog.h"
#include <QString>
#include <QMessageBox>

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
    registrationDialog* regDial = new registrationDialog(this);
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
    return false;
}

