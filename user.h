#ifndef USER_H
#define USER_H

#include <QObject>
#include <QString>

class User : public QObject
{
    Q_OBJECT
public:
    explicit User(QString email, QString username, QString password, QObject *parent = nullptr);
    ~User();
    static int userCount;

private:
    int m_id;
    QString m_email;
    QString m_username;
    QString m_password;

signals:

};

#endif // USER_H
