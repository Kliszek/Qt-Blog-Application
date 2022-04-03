#include "user.h"

User::User(const User &user, QObject *parent)
{
    m_email = user.m_email;
    m_username = user.m_username;
    m_password = user.m_password;
}

User::User(QString email, QString username, QString password, QObject *parent)
{
    m_email = email;
    m_username = username;
    m_password = password;
}

User::~User()
{

}

User &User::operator=(const User &user)
{
    m_email = user.m_email;
    m_username = user.m_username;
    m_password = user.m_password;
    return *this;
}
