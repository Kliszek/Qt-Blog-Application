#ifndef BLOGENTRY_H
#define BLOGENTRY_H

#include <QObject>
#include <QString>
#include <QDate>
#include <QJsonObject>

class BlogEntry : public QObject
{
    Q_OBJECT
public:
    explicit BlogEntry(QObject *parent = nullptr){};
    explicit BlogEntry(const BlogEntry& blogEntry, QObject *parent = nullptr);
    explicit BlogEntry(QString title, QDate date, QString content, QObject *parent = nullptr);
    ~BlogEntry();

    BlogEntry& operator=(const BlogEntry& blogEntry);

    QJsonObject toJson() const {
        return {{"title", m_title}, {"date", m_date.toString()}, {"content", m_content}};
    }

private:
    QString m_title;
    QDate m_date;
    QString m_content;

signals:

};

#endif // BLOGENTRY_H
