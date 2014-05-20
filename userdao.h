#ifndef USERDAO_H
#define USERDAO_H
#include <user.h>
#include <QString>
#include <QtSql/QSqlQuery>
#include <QVariant>
#include <QtSqlDepends>
#include <QSqlRecord>
#include <databaseconnection.h>
#include <QSharedPointer>
#include <QJsonObject>
#include <QVariantList>



class UserDAO
{
public:
    UserDAO();
    ~UserDAO();
    JMap Create (PUser user);
    JMap UserDetails(QString sEmail);
    JMap UpdateUser(QString sAbout, QString sEmail, QString sName);
    JMap FollowUser(QString sUserEmail, QString sFollowEmail);
    JMap UnFollowUser(QString sUserEmail, QString sFollowEmail);
    QVariantList  ListFollow(JMap input);
    QVariantList  ListFollowers(JMap input);
    QVariantList  ListPosts(JMap input);

private:
    PDatabaseConnection   m_pConnection;
    QString               m_sName;
};

typedef QSharedPointer<UserDAO> PUserDAO;
#endif // USERDAO_H
