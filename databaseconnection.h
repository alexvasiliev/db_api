#ifndef DATABASECONNECTION_H
#define DATABASECONNECTION_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QMap>
#include <QVariant>
#include <QSharedPointer>

class DatabaseConnection
{
public:
    DatabaseConnection(QString sName = "");
    ~DatabaseConnection();
    QSqlDatabase GetConnection();

private:
    QSqlDatabase* m_pConnection;
    QString       m_sConnectionName;
};
typedef QSharedPointer<DatabaseConnection> PDatabaseConnection;
typedef QMap<QString, QVariant> JMap;
#endif // DATABASECONNECTION_H
