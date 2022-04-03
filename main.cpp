#include "logindialog.h"
#include "usermanager.h"

#include <QApplication>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LoginDialog w;
    w.show();
    UserManager::loadUsers();
    return a.exec();
}
