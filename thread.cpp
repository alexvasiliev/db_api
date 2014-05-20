#include "thread.h"

Thread::Thread(QString sTitle, QDateTime date, QString sMessage, QString sSlug,
               QString sForum, QString sCreatorEmail, bool bIsClosed, bool bIsDeleted)

    :   m_sTitle(sTitle)
    ,   m_date(date)
    ,   m_sMessage(sMessage)
    ,   m_sSlug(sSlug)
    ,   m_sForum(sForum)
    ,   m_sCreatorEmail(sCreatorEmail)
    ,   m_bIsClosed(bIsClosed)
    ,   m_bIsDeleted(bIsDeleted)
    ,   m_nId(-1)
{
}

Thread::Thread()
    :   m_nId(-1)
{
}
