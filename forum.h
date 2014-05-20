#ifndef FORUM_H
#define FORUM_H

#include <QString>
#include <QSharedPointer>

class Forum
{
public:
    Forum(QString sName, QString sShortName, QString sCreatorEmail, bool bIsDeleted = 0);
    Forum();
//private:
    int m_nId;
    QString m_sName;
    QString m_sShortName;
    QString m_sCreatorEmail;
    bool    m_bIsDeleted;  
    
};
typedef QSharedPointer<Forum> PForum;
#endif // FORUM_H
