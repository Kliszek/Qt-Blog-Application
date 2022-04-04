#include "maindialog.h"
#include "ui_maindialog.h"

MainDialog::MainDialog(const User *user, QWidget *parent) :QDialog(parent), ui(new Ui::MainDialog), m_currentUser(user)
{
    ui->setupUi(this);
    ui->txtUsername->setText(m_currentUser->m_username);
}

MainDialog::~MainDialog()
{
    delete ui;
}

void MainDialog::on_buttonBox_accepted()
{
    BlogManager::loadBlogs();
    BlogManager::saveBlogs();
}

