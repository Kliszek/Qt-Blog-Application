#include "registrationdialog.h"
#include "ui_registrationdialog.h"

registrationDialog::registrationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::registrationDialog)
{
    ui->setupUi(this);
}

registrationDialog::~registrationDialog()
{
    delete ui;
}
