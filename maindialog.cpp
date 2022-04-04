#include "maindialog.h"
#include "ui_maindialog.h"

MainDialog::MainDialog(const User *user, QWidget *parent) :QDialog(parent), ui(new Ui::MainDialog), m_currentUser(user)
{
    ui->setupUi(this);
    ui->txtUsername->setText(m_currentUser->m_username);
}

MainDialog::~MainDialog()
{
    qInfo() << "Main window deconstructed";
    delete ui;
}

void MainDialog::on_btnBox_accepted()
{
    BlogManager::loadBlogs();
    BlogManager::saveBlogs();
}


void MainDialog::on_btnBox_rejected()
{
    QSettings settings(QSettings::UserScope);
    settings.clear();
    this->close();
    this->parentWidget()->show();
    delete(this);
}

