#include "forum.h"

Forum::Forum(QString sName, QString sShortName, QString sCreatorEmail, bool bIsDeleted)
    : m_sName(sName)
    , m_sShortName(sShortName)
    , m_sCreatorEmail(sCreatorEmail)
    , m_bIsDeleted(bIsDeleted)
    ,   m_nId(-1)
{
}

Forum::Forum()
    :   m_nId(-1)
{
}
