#ifndef FORUMDAO_H
#define FORUMDAO_H
#include "forum.h"
#include <QString>
#include <QtSql/QSqlQuery>
#include <QVariant>
#include <QtSqlDepends>
#include <QSqlRecord>
#include <databaseconnection.h>
#include <QSharedPointer>
#include <QJsonObject>
#include <QJsonArray>
#include <userdao.h>
class ForumDAO
{
public:
    ForumDAO();
    JMap Create     (PForum forum);
    JMap ForumDetails(JMap input);


private:
    //PDatabaseConnection m_pConnection;
    QString         m_sName;
};

typedef QSharedPointer<ForumDAO> PForumDAO;
#endif // FORUMDAO_H
