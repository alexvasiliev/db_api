#include "user.h"

User::User()
    :   m_nId(-1)
{
}

User::User(QString sName, QString sUsername, QString sEmail, QString sAbout, bool bIsAnonim)
    :   m_sName(sName)
    ,   m_sUserName(sUsername)
    ,   m_sEmail(sEmail)
    ,   m_sAbout(sAbout)
    ,   m_bIsAnonim(bIsAnonim)
    ,   m_nId(-1)
{
}
