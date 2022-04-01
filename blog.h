#ifndef BLOG_H
#define BLOG_H

#include <QObject>
#include <QString>
#include <QList>
#include "blogentry.h"

class Blog : public QObject
{
    Q_OBJECT
public:
    explicit Blog(int blogId, int ownerId, QString title, QList<BlogEntry*> entryList, QObject *parent = nullptr);
    ~Blog();

private:
    int m_blogId;
    int m_ownerId;
    QString m_title;
    QList<BlogEntry*> m_entryList;

signals:

};

#endif // BLOG_H
