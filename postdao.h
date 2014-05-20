#ifndef POSTDAO_H
#define POSTDAO_H
#include <post.h>
#include <QString>
#include <QtSql/QSqlQuery>
#include <QVariant>
#include <QtSqlDepends>
#include <QSqlRecord>
#include <databaseconnection.h>
#include <QSharedPointer>
#include <QJsonObject>
#include <QJsonArray>

class PostDAO
{
public:
    PostDAO();
    JMap Create (PPost post);
    JMap PostDetails (JMap input);
    JMap Update (int nId, QString sMessage);
    JMap Remove_Restore(int nId, int is_delete);
    JMap Vote(int nId, int vote);
    QVariantList List(JMap input);
    QVariantList ForumListUsers(JMap input);
    QVariantList ForumListPosts(JMap input);
    QVariantList ForumListThreads(JMap input);
    QVariantList ThreadsListPosts(JMap input);
    QVariantList UserListPosts(JMap input);
    void ClearDatabase();

private:
    //PDatabaseConnection   m_pConnection;
    QString         m_sName;
};
typedef QSharedPointer<PostDAO> PPostDAO;

#endif // POSTDAO_H
