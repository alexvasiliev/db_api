#ifndef THREADDAO_H
#define THREADDAO_H
#include <QString>
#include <QSqlQuery>
#include <QVariant>
#include <QSqlDriver>
#include <QSqlDriverPlugin>
#include <QSql>
#include <QtSqlDepends>
#include <databaseconnection.h>
#include <thread.h>

class ThreadDAO
{
public:
    ThreadDAO();
    JMap Create (PThread pThread);
    JMap Details(JMap input);
    JMap Close_Open(int nId, int Close);
    JMap Remove_Restore(int nId, int is_delete);
    JMap Update(int nId, QString sMessage, QString sSlug);
    JMap Vote(int nId, int vote);
    JMap Subscribe(QString sUser, int nThreadId);
    JMap UnSubscribe(QString sUser, int nThreadId);
    QVariantList List(JMap input);
private:
    PDatabaseConnection   m_pConnection;
    QString m_sName;
};

typedef QSharedPointer<ThreadDAO> PThreadDAO;
#endif // THREADDAO_H
