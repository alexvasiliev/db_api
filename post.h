#ifndef POST_H
#define POST_H
#include <QString>
#include <QDateTime>
#include <QSharedPointer>

class Post
{
public:
    Post(QDateTime date, QString sMessage, QString sCreatorEmail, int nThreadId, QString sForum, int nParentId = -1, bool bisAproved = 0, bool bIsHighlighted = 0,
         bool bIsEdited = 0, bool bIsSpam = 0, bool bIsDeleted = 0);
    Post();
//private:
    int         m_nId;
    QDateTime   m_date;
    QString     m_sMessage;
    QString     m_sCreatorEmail;
    int         m_nTHreadId;
    QString     m_sForum;
    int         m_nPoints;
    int         m_nParentId;
    bool        m_bIsAproved;
    bool        m_bIsHighlighted;
    bool        m_bIsEdited;
    bool        m_bIsSpam;
    bool        m_bIsDeleted;
    bool        m_bIsDeletedParent;
    bool        m_bIsDeletedThread;
    int         m_nLikes;
    int         m_nDisLikes;
};

typedef QSharedPointer<Post> PPost;

#endif // POST_H
