#ifndef USER_H
#define USER_H
#include <QString>
#include <QSharedPointer>

class User
{
public:
    User();
    User(QString sName, QString sUsername, QString sEmail, QString sAbout = "", bool  bIsAnonim = 0);
//private:
    int         m_nId;
    QString     m_sName;
    QString     m_sUserName;
    QString     m_sEmail;
    QString     m_sAbout;
    bool        m_bIsAnonim;
};

typedef QSharedPointer<User> PUser;
#endif // USER_H
