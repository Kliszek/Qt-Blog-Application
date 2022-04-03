#include "blog.h"

Blog::Blog(int blogId, int ownerId, QString title, QList<BlogEntry *> entryList, QObject *parent)
{
    m_blogId = blogId;
    m_ownerId = ownerId;
    m_title = title;
    m_entryList = entryList;
}

Blog::~Blog()
{

}
