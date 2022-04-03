#include "blogmanager.h"

QList<Blog>* BlogManager::blogList;

BlogManager::BlogManager(QObject *parent)
    : QObject{parent}
{

}

bool BlogManager::loadBlogs()
{


    return true;
}

bool BlogManager::saveBlogs()
{

//    BlogEntry be("title123", QDate::currentDate(), "content123456");
//    BlogEntry be2("title234532", QDate::currentDate(), "fefearf");

//    Blog blogTest(123, 4, "blog title", new QList<BlogEntry>());

//    blogTest.m_entryList->append(be);
//    blogTest.m_entryList->append(be2);

//    blogList = new QList<Blog>();

//    blogList->append(blogTest);



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
