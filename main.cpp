#include "dialog.h"
#include "usermanager.h"

#include <QApplication>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog w;
    w.show();
    UserManager::loadUsers();
    return a.exec();
}
