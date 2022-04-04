#ifndef BLOG_H
#define BLOG_H

#include <QObject>
#include <QString>
#include <QList>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include "blogentry.h"

class Blog : public QObject
{
    Q_OBJECT

friend class BlogManager;

public:
    explicit Blog(bool generateId = false, QObject *parent = nullptr);
    explicit Blog(const Blog& blog, QObject *parent = nullptr);
    explicit Blog(QString blogId, QString ownerId, QString title, QList<BlogEntry>* entryList, QObject *parent = nullptr);
    ~Blog();
    Blog& operator=(const Blog& blog);


    QJsonObject toJson() const;


    QList<BlogEntry>* m_entryList;

    QString m_ownerId;
    QString m_title;

    const QString &getBlogId() const;

private:
    QString m_blogId;

signals:

};

#endif // BLOG_H
