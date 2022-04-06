#include "blogentry.h"

BlogEntry::BlogEntry(const BlogEntry& blogEntry, QObject *parent)
{
    m_title = blogEntry.m_title;
    m_date = blogEntry.m_date;
    m_content = blogEntry.m_content;
}

BlogEntry::BlogEntry(QString title, QDateTime date, QString content, QObject *parent)
{
    m_title = title;
    m_date = date;
    m_content = content;
}

BlogEntry::~BlogEntry()
{
    qInfo() << "Deconstructed BlogEntry: " + m_title;
//    qInfo() << m_title + " " + m_content;
}

BlogEntry &BlogEntry::operator=(const BlogEntry &blogEntry)
{
    m_title = blogEntry.m_title;
    m_date = blogEntry.m_date;
    m_content = blogEntry.m_content;
    return *this;
}
