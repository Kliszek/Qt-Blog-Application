#include "logindialog.h"
#include "maindialog.h"
#include "usermanager.h"

#include <QApplication>
#include <QString>
#include <QSettings>
#include <QDebug>


int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("EGUI Jakub Kliszko");
    QCoreApplication::setApplicationName("EGUI Qt Blog Application");

    QApplication a(argc, argv);
    QSettings::setDefaultFormat(QSettings::IniFormat);

    UserManager::loadUsers();

    qInfo() << QCoreApplication::applicationName();

    QSettings settings(QSettings::UserScope);
    QString currentlyLogged = settings.value("logged", "").toString();

    const User* loggedUser = UserManager::getUserById(currentlyLogged);

    LoginDialog w(loggedUser);


    return a.exec();
}
