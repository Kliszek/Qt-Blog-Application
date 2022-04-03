#ifndef BLOGMANAGER_H
#define BLOGMANAGER_H

#include <QObject>
#include <QString>
#include <QMessageBox>
#include <QDebug>
#include <QList>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QIODevice>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QTextStream>
#include "blog.h"
#include "blogentry.h"
#include "usermanager.h"

class BlogManager : public QObject
{
    Q_OBJECT
public:
    explicit BlogManager(QObject *parent = nullptr);

    static bool loadBlogs();

    static bool saveBlogs();

    static void addBlog(const Blog& blog);

private:
    static QList<Blog>* blogList;

signals:

};

#endif // BLOGMANAGER_H
