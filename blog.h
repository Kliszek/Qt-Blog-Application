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
public:
    explicit Blog(QObject *parent = nullptr){};
    explicit Blog(const Blog& blog, QObject *parent = nullptr);
    explicit Blog(int blogId, int ownerId, QString title, QList<BlogEntry>* entryList, QObject *parent = nullptr);
    ~Blog();
    Blog& operator=(const Blog& blog);


    QJsonObject toJson() const;


    QList<BlogEntry>* m_entryList;

    int m_blogId;
    int m_ownerId;
    QString m_title;

signals:

};

#endif // BLOG_H
