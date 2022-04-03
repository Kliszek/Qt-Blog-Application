#include "usermanager.h"

QList<User>* UserManager::userList;

UserManager::UserManager(QObject *parent)
    : QObject{parent}
{

}

QList<User>* UserManager::getUserList()
{
    if(userList == nullptr)
        userList = new QList<User>();

    return userList;
}

bool UserManager::createDirectory(QDir& userDir)
{

    userDir = QDir::home();

    if(!userDir.exists("EGUI_Qt_Blog_Application"))
        if(!userDir.mkdir("EGUI_Qt_Blog_Application"))
        {
            QMessageBox::critical(nullptr, "Error", "Could not create a folder for user data!");
            return false;
        }
    userDir.cd("EGUI_Qt_Blog_Application");

    return true;
}



bool UserManager::loadUsers()
{
    delete userList;
    userList = new QList<User>();

    QDir userDir;

    if(!createDirectory(userDir))
        return false;

    QFile userFile = userDir.absolutePath() + "/users.json";


    if(!userFile.exists())
        return true;

    userFile.open(QIODevice::ReadOnly);

    QString userListJson = userFile.readAll();

    userFile.close();

    //qInfo() << userListJson;

    QJsonDocument jsonDoc = QJsonDocument::fromJson(userListJson.toUtf8());
    QJsonArray jsonArr = jsonDoc.array();

    //qInfo() << jsonArr;
    foreach(const QJsonValue& jsonVal, jsonArr)
    {
        QJsonObject jsonObj = jsonVal.toObject();

        User newUser;
        newUser.m_username = jsonObj["username"].toString();
        newUser.m_email = jsonObj["email"].toString();
        newUser.m_password = jsonObj["password"].toString();

        userList->append(newUser);
    }

    return true;
}

bool UserManager::saveUsers()
{
    QJsonArray jsonArr;

    for(int i=0; i<userList->size(); i++)
    {
        jsonArr.append(userList->at(i).toJson());
    }

    QDir userDir;

    if(!createDirectory(userDir))
        return false;

    QFile userFile = userDir.absolutePath() + "/users.json";

    userFile.open(QIODevice::WriteOnly);

    QTextStream stream(&userFile);
    stream << QJsonDocument(jsonArr).toJson();
    userFile.close();

    return true;
}

bool UserManager::checkAvailability(QString username, QString email)
{
    if(userList == nullptr)
    {
        qCritical() << "Trying to check available name, but the user list has not been loaded yet!";
        return false;
    }

    for(int i=0; i<userList->size(); i++)
    {
        if(userList->at(i).m_username == username || userList->at(i).m_email == email)
            return false;
    }

    return true;
}

QString UserManager::getPassword(const QString username)
{
    if(userList == nullptr)
    {
        qCritical() << "Trying to check available name, but the user list has not been loaded yet!";
        return "";
    }

    for(int i=0; i<userList->size(); i++)
    {
        if(userList->at(i).m_username == username)
            return userList->at(i).m_password;
    }
    qCritical() << "User with provided name does not exist!";
    return "";
}
