#include "databaseconnection.h"

DatabaseConnection::DatabaseConnection(QString sName)
    :m_sConnectionName(sName)
{   
    m_pConnection = new QSqlDatabase(QSqlDatabase::addDatabase("QMYSQL", m_sConnectionName));
    m_pConnection->setHostName("localhost");
    m_pConnection->setPort(3036);
    m_pConnection->setUserName("root");
    m_pConnection->setPassword("qwerty");
    m_pConnection->setDatabaseName("db_homework");
    if(!m_pConnection->isOpen())
    {
         m_pConnection->open();
    }
}


DatabaseConnection::~DatabaseConnection()
{
    if(m_pConnection->isOpen())
    {
         m_pConnection->close();
    }
}

QSqlDatabase DatabaseConnection::GetConnection()
{
    return *m_pConnection;
}
