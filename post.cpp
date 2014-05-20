#include "post.h"

Post::Post(QDateTime date, QString sMessage, QString sCreatorEmail, int nThreadId, QString sForum, int nParentId,
           bool bisAproved, bool bIsHighlighted, bool bIsEdited, bool bIsSpam, bool bIsDeleted)
    :   m_date(date)
    ,   m_sMessage(sMessage)
    ,   m_sCreatorEmail(sCreatorEmail)
    ,   m_nTHreadId(nThreadId)
    ,   m_sForum(sForum)
    ,   m_nParentId(nParentId)
    ,   m_bIsAproved(bisAproved)
    ,   m_bIsHighlighted(bIsHighlighted)
    ,   m_bIsEdited(bIsEdited)
    ,   m_bIsSpam(bIsSpam)
    ,   m_bIsDeleted(bIsDeleted)
    ,   m_nId(-1)
{
}


Post::Post()
    :   m_nId(-1)
{
}
