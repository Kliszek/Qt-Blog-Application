#include "blog.h"

Blog::Blog(bool generateId, QObject *parent)
{
    if(generateId)
    {
        m_blogId = QUuid::createUuid().toString(QUuid::Id128);
    }
}

Blog::Blog(const Blog& blog, QObject *parent)
{
    m_blogId = blog.m_blogId;
    m_ownerId = blog.m_ownerId;
    m_title = blog.m_title;
    m_entryList = blog.m_entryList;
}

Blog::Blog(QString blogId, QString ownerId, QString title, QList<BlogEntry>* entryList, QObject *parent)
{
    m_blogId = blogId;
    m_ownerId = ownerId;
    m_title = title;
    m_entryList = entryList;
}

Blog::~Blog()
{

}

Blog &Blog::operator=(const Blog &blog)
{
    m_blogId = blog.m_blogId;
    m_ownerId = blog.m_ownerId;
    m_title = blog.m_title;
    m_entryList = blog.m_entryList;
    return *this;
}

QJsonObject Blog::toJson() const
{
    QJsonArray jsonArr;

    for(int i=0; i< m_entryList->size(); i++)
    {
        jsonArr.append( m_entryList->at(i).toJson() );
    }

    QJsonObject jsonObj;
    jsonObj.insert("blogId", m_blogId);
    jsonObj.insert("ownerId", m_ownerId);
    jsonObj.insert("title", m_title);
    jsonObj.insert("entryList", jsonArr);

    return jsonObj;
}

const QString &Blog::getBlogId() const
{
    return m_blogId;
}
