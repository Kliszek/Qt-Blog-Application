#ifndef BLOGENTRY_H
#define BLOGENTRY_H

#include <QObject>
#include <QString>
#include <QDate>

class BlogEntry : public QObject
{
    Q_OBJECT
public:
    explicit BlogEntry(QString title, QDate date, QString content, QObject *parent = nullptr);
    ~BlogEntry();

private:
    QString m_title;
    QDate m_date;
    QString m_content;

signals:

};

#endif // BLOGENTRY_H
