#ifndef USER_H
#define USER_H

#include <QObject>
#include <QString>
#include <QJsonObject>
#include <QList>

class User : public QObject
{
    Q_OBJECT
public:
    explicit User(QObject *parent = nullptr){};
    explicit User(const User& user, QObject *parent = nullptr);
    explicit User(QString email, QString username, QString password, QObject *parent = nullptr);
    ~User();
    User& operator=(const User& user);
    static int userCount;

    static QList<User>* userList;
    static QList<User>* getUserList() {return userList;}

    QJsonObject toJson() const {
        return {{"email", m_email}, {"username", m_username}, {"password", m_password}};
    }

    QString m_email;
    QString m_username;
    QString m_password;

private:
    int m_id;

signals:

};

#endif // USER_H
