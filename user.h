#ifndef USER_H
#define USER_H

#include <QObject>
#include <QString>
#include <QJsonObject>
#include <QList>
#include <QUuid>

class User : public QObject
{
    Q_OBJECT

friend class UserManager;

public:
    explicit User(bool generateId = false, QObject *parent = nullptr);
    explicit User(const User& user, QObject *parent = nullptr);
    explicit User(QString id, QString email, QString username, QString password, QObject *parent = nullptr);
    ~User();
    User& operator=(const User& user);


    QJsonObject toJson() const {
        return {{"id", m_id}, {"email", m_email}, {"username", m_username}, {"password", m_password}};
    }

    QString m_email;
    QString m_username;
    QString m_password;

    const QString &getId() const;

private:
    QString m_id;

signals:

};

#endif // USER_H
