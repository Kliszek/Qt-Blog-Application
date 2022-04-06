#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <QObject>
#include <QString>
#include <QMessageBox>
#include <QDebug>
#include <QList>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QIODevice>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QTextStream>
#include <QSettings>
#include "user.h"

class UserManager : public QObject
{
    Q_OBJECT
public:
    explicit UserManager(QObject *parent = nullptr);

    static QList<User>* getUserList();

    static bool createDirectory(QDir& userDir);

    static bool loadUsers();

    static bool saveUsers();

    static const User* getUserByName(const QString username);

    static const User* getUserById(const QString id);

    static bool usernameAvailable(QString username);
    static bool emailAvailable(QString email);
    static bool idAvailable(QString id);

    static QString getPassword(const QString username);

private:
    static QList<User>* userList;

signals:

};

#endif // USERMANAGER_H
