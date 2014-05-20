#include "forumdao.h"

ForumDAO::ForumDAO()
{

}


JMap     ForumDAO::Create(PForum forum)
{
    JMap jmap;
    QString queryString1 = QString("INSERT INTO Forums(name, short_name, creator_email) VALUES (\"%1\", \"%2\", \"%3\")")
            .arg(forum->m_sName)
            .arg(forum->m_sShortName)
            .arg(forum->m_sCreatorEmail);
    QSqlQuery query(QSqlDatabase::database("mysqlcon"));
    if (query.exec(queryString1))
        forum->m_nId = query.lastInsertId().toInt();
    else
    {
        QSqlError temp = query.lastError();
        jmap.insert("error" , temp.text());
        return jmap;
    }

    jmap.insert("id", forum->m_nId);
    jmap.insert("name", forum->m_sName);
    jmap.insert("short_name", forum->m_sShortName);
    jmap.insert("user", forum->m_sCreatorEmail);
    return jmap;

}

JMap ForumDAO::ForumDetails(JMap input)
{
    QSqlQuery query(QSqlDatabase::database("mysqlcon"));
    JMap jmap;
    Forum forum;
    QString short_name = input.take("short_name").toString();
    QString queryString1 = QString("SELECT * FROM Forums WHERE short_name = \"%1\"")
            .arg(short_name);
    if(query.exec(queryString1))
    {
        while(query.next())
        {
            forum.m_nId = query.value(query.record().indexOf("id")).toInt();
            forum.m_sName = query.value(query.record().indexOf("name")).toString();
            forum.m_sShortName = short_name;
            forum.m_sCreatorEmail = query.value(query.record().indexOf("creator_email")).toString();
        }
    }
    else
    {
        QSqlError temp = query.lastError();
        jmap.insert("error" , temp.text());
        return jmap;
    }
    jmap.insert("id",forum.m_nId);
    jmap.insert("name", forum.m_sName);
    jmap.insert("short_name", forum.m_sShortName);
    //QString user_string;
    if(input.contains("user"))
    {
        UserDAO userDAO;
        JMap user = userDAO.UserDetails(forum.m_sCreatorEmail);
        jmap.insert("user", user);
    }else{
        jmap.insert("user", forum.m_sCreatorEmail);
    }
    return jmap;
}
