#include "blogentry.h"

BlogEntry::BlogEntry(QString title, QDate date, QString content, QObject *parent)
{
    m_title = title;
    m_date = date;
    m_content = content;
}

BlogEntry::~BlogEntry()
{

}
