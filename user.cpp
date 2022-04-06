#include "user.h"

User::User(bool generateId, QObject *parent)
{
    if(generateId)
    {
        m_id = QUuid::createUuid().toString(QUuid::Id128);
    }
}

User::User(const User &user, QObject *parent)
{
    m_email = user.m_email;
    m_username = user.m_username;
    m_password = user.m_password;
    m_id = user.m_id;
}

User::User(QString id, QString email, QString username, QString password, QObject *parent)
{
    m_email = email;
    m_username = username;
    m_password = password;
    m_id = id;
}

User::~User()
{
    qInfo() << "Deconstructed User: " + m_username;
}

User &User::operator=(const User &user)
{
    m_email = user.m_email;
    m_username = user.m_username;
    m_password = user.m_password;
    m_id = user.m_id;
    return *this;
}

const QString &User::getId() const
{
    return m_id;
}
