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

    static const User* getUser(const QString username);

    static bool checkAvailability(QString username, QString email, QString id);

    static QString getPassword(const QString username);

private:
    static QList<User>* userList;

signals:

};

#endif // USERMANAGER_H
