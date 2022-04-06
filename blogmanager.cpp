#include "blogmanager.h"

QList<Blog>* BlogManager::blogList;

BlogManager::BlogManager(QObject *parent)
    : QObject{parent}
{

}

bool BlogManager::loadBlogs()
{
    delete blogList;
    blogList = new QList<Blog>();

    QDir userDir;

    if(!UserManager::createDirectory(userDir))
        return false;

    QFile blogFile = userDir.absoluteFilePath("blogs.json");


    if(!blogFile.exists())
        return true;

    blogFile.open(QIODevice::ReadOnly);

    QString blogListJson = blogFile.readAll();

    blogFile.close();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(blogListJson.toUtf8());
    QJsonArray jsonArr = jsonDoc.array();

    foreach(const QJsonValue& jsonVal, jsonArr)
    {
        QJsonObject jsonObj = jsonVal.toObject();

        Blog newBlog;
        newBlog.m_title = jsonObj["title"].toString();
        newBlog.m_ownerId = jsonObj["ownerId"].toString();
        newBlog.m_blogId = jsonObj["blogId"].toString();
        newBlog.m_entryList = QSharedPointer< QList<BlogEntry> >(new QList<BlogEntry>());

        QJsonArray blogEntries = jsonObj["entryList"].toArray();

        foreach(const QJsonValue& blogEntryJson, blogEntries)
        {
            BlogEntry newBlogEntry(&newBlog);
            newBlogEntry.m_title = blogEntryJson["title"].toString();
            newBlogEntry.m_content = blogEntryJson["content"].toString();
            newBlogEntry.m_date = QDateTime::fromString(blogEntryJson["date"].toString());
            //qInfo() << newBlogEntry.toJson();
            newBlog.m_entryList->append(newBlogEntry);
        }

        blogList->append(newBlog);
        //qInfo() << newBlog.toJson();
    }

    return true;
}

bool BlogManager::saveBlogs()
{

    QJsonArray jsonArr;

    for(int i=0; i<blogList->size(); i++)
    {
        jsonArr.append(blogList->at(i).toJson());
    }

    QDir userDir;

    if(!UserManager::createDirectory(userDir))
        return false;

    QFile blogFile = userDir.absolutePath() + "/blogs.json";

    blogFile.open(QIODevice::WriteOnly);

    QTextStream stream(&blogFile);
    stream << QJsonDocument(jsonArr).toJson();
    blogFile.close();

    return true;
}

void BlogManager::addBlog(const Blog &blog)
{
    blogList->append(blog);

    if(!saveBlogs())
    {
        QMessageBox::critical(nullptr, "Error", "Could not save the blog to the blog file!");
    }
}

const Blog* BlogManager::getBlogByTitle(const QString blogtitle)
{
    if(blogList == nullptr)
    {
        qCritical() << "Trying to get blog by title, but the blog list has not been loaded yet!";
        return nullptr;
    }

    for(int i=0; i<blogList->size(); i++)
    {
        if(blogList->at(i).m_title == blogtitle)
        {
            return &blogList->at(i);
        }
    }

    return nullptr;
}

const Blog *BlogManager::getBlogById(const QString id)
{
    if(blogList == nullptr)
    {
        qCritical() << "Trying to get blog by id, but the blog list has not been loaded yet!";
        return nullptr;
    }

    for(int i=0; i<blogList->size(); i++)
    {
        if(blogList->at(i).m_blogId == id)
        {
            return &blogList->at(i);
        }
    }

    return nullptr;
}

bool BlogManager::idAvailable(QString id)
{
    if(blogList == nullptr)
    {
        qCritical() << "Trying to check available blog id, but the blog list has not been loaded yet!";
        return false;
    }

    for(int i=0; i<blogList->size(); i++)
    {
        if(blogList->at(i).m_blogId == id)
            return false;
    }

    return true;
}
bool BlogManager::titleAvailable(QString title)
{
    if(blogList == nullptr)
    {
        qCritical() << "Trying to check available blog title, but the blog list has not been loaded yet!";
        return false;
    }

    for(int i=0; i<blogList->size(); i++)
    {
        if(blogList->at(i).m_title == title)
            return false;
    }

    return true;
}

QList<Blog> *BlogManager::getBlogList()
{
    return blogList;
}
