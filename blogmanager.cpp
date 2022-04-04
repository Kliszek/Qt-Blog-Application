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

    QFile blogFile = userDir.absolutePath() + "/blogs.json";


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
            BlogEntry newBlogEntry;
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

//    BlogEntry be("title123", QDateTime::currentDateTime(), "content123456");
//    BlogEntry be2("title234532", QDateTime::currentDateTime(), "dodawane");

//    Blog blogTest("123", "4", "blog title", new QList<BlogEntry>());

//    blogTest.m_entryList->append(be);
//    blogTest.m_entryList->append(be2);

//    //blogList = new QList<Blog>();

//    blogList->append(blogTest);
//    blogList->pop_back();

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
