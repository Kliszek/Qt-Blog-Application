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

bool UserManager::createDirectory(QDir& dataDir)
{

    QSettings settings;
    QString dataDirString = settings.value("dataDir", QDir::home().absolutePath()+"/EGUI_Qt_Blog_Application/").toString();
    settings.setValue("dataDir", dataDirString);

    dataDir = QDir(dataDirString);

    if(!dataDir.exists())
        if(!dataDir.mkpath("."))
        {
            QMessageBox::critical(nullptr, "Error", "Could not create a folder for user data!");
            return false;
        }

    return true;
}



bool UserManager::loadUsers()
{
    delete userList;
    userList = new QList<User>();

    QDir userDir;

    if(!createDirectory(userDir))
    {
        QMessageBox::critical(nullptr, "Error", "Could not create the data folder for json files!\r\nPlease choose another location!");
        return false;
    }

    QFile userFile = userDir.absoluteFilePath("users.json");

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
        newUser.m_id = jsonObj["id"].toString();

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

    QFile userFile = userDir.absoluteFilePath("/users.json");

    if(!QFileInfo(userFile).isWritable())
    {
        QMessageBox::critical(nullptr, "Error", "Cannot save file users.json!\r\nPlease choose another location!");
        return false;
    }

    userFile.open(QIODevice::WriteOnly);

    QTextStream stream(&userFile);
    stream << QJsonDocument(jsonArr).toJson();
    userFile.close();

    return true;
}

const User* UserManager::getUserByName(const QString username)
{
    if(userList == nullptr)
    {
        qCritical() << "Trying to get user by name, but the user list has not been loaded yet!";
        return nullptr;
    }

    for(int i=0; i<userList->size(); i++)
    {
        if(userList->at(i).m_username == username)
        {
            return &userList->at(i);
        }
    }

    return nullptr;
}

const User *UserManager::getUserById(const QString id)
{
    if(userList == nullptr)
    {
        qCritical() << "Trying to get user by id, but the user list has not been loaded yet!";
        return nullptr;
    }

    for(int i=0; i<userList->size(); i++)
    {
        if(userList->at(i).m_id == id)
        {
            return &userList->at(i);
        }
    }

    return nullptr;
}

bool UserManager::usernameAvailable(QString username)
{
    if(userList == nullptr)
    {
        qCritical() << "Trying to check available name, but the user list has not been loaded yet!";
        return false;
    }

    for(int i=0; i<userList->size(); i++)
    {
        if(userList->at(i).m_username == username)
            return false;
    }

    return true;
}
bool UserManager::emailAvailable(QString email)
{
    if(userList == nullptr)
    {
        qCritical() << "Trying to check available email, but the user list has not been loaded yet!";
        return false;
    }

    for(int i=0; i<userList->size(); i++)
    {
        if(userList->at(i).m_email == email)
            return false;
    }

    return true;
}
bool UserManager::idAvailable(QString id)
{
    if(userList == nullptr)
    {
        qCritical() << "Trying to check available id, but the user list has not been loaded yet!";
        return false;
    }

    for(int i=0; i<userList->size(); i++)
    {
        if(userList->at(i).m_id == id)
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
