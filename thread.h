#ifndef THREAD_H
#define THREAD_H
#include <QString>
#include <QDateTime>

class Thread
{
public:
    Thread(QString sTitle, QDateTime date, QString sMessage, QString sSlug,
           QString sForum, QString sCreatorEmail, bool b_IsClosed = 0, bool bIsDeleted = 0);
    Thread();
//private:
    int         m_nId;
    QString     m_sTitle;
    QString     m_sMessage;
    QString     m_sSlug;
    QString     m_sCreatorEmail;
    QDateTime   m_date;
    QString     m_sForum;
    int         m_nPoints;
    bool        m_bIsClosed;
    bool        m_bIsDeleted;
    bool        m_bIsDeletedByParent;
    int         m_nDislikes;
    int         m_nLikes;
};

typedef QSharedPointer<Thread> PThread;
#endif // THREAD_H
