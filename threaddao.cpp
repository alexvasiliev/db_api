#include "threaddao.h"
#include "userdao.h"
#include "forumdao.h"

ThreadDAO::ThreadDAO()
{
    /*m_sName = "ThreadDAO";
    m_pConnection = PDatabaseConnection(new DatabaseConnection(m_sName));*/

    //m_pConnection = DatabaseConnection::GetConnection();
}

JMap ThreadDAO::Create(PThread thread)
{
    JMap jmap;
    QString queryString1 = QString("INSERT INTO Threads(title, isClosed, date, message, slug, forum_short_name,\
                                   creator_email, isDeleted) \
            VALUES (\"%1\", %2, \"%3\", \"%4\", \"%5\", \"%6\", \"%7\", %8)")
            .arg(thread->m_sTitle)
            .arg(QVariant(QVariant(thread->m_bIsClosed).toInt()).toString())
            .arg(thread->m_date.toString("yyyy-MM-dd HH:mm:ss"))
            .arg(thread->m_sMessage)
            .arg(thread->m_sSlug)
            .arg(thread->m_sForum)
            .arg(thread->m_sCreatorEmail)
            .arg(QVariant(QVariant(thread->m_bIsDeleted).toInt()).toString());
    QSqlQuery query(QSqlDatabase::database("mysqlcon"));
    if (query.exec(queryString1))
    {
        thread->m_nId = query.lastInsertId().toInt();
    }
    else
    {
        QSqlError temp = query.lastError();
        jmap.insert("error" , temp.text());
        return jmap;
    }
    jmap.insert("date", thread->m_date.toString("yyyy-MM-dd HH:mm:ss"));
    jmap.insert("forum", thread->m_sForum);
    jmap.insert("id", thread->m_nId);
    jmap.insert("isClosed", thread->m_bIsClosed);
    jmap.insert("isDeleted", thread->m_bIsDeleted);
    jmap.insert("message", thread->m_sMessage);
    jmap.insert("slug", thread->m_sSlug);
    jmap.insert("title", thread->m_sTitle);
    jmap.insert("user", thread->m_sCreatorEmail);
    return jmap;
}

JMap ThreadDAO::Details(JMap input)
{
    QSqlQuery query(QSqlDatabase::database("mysqlcon"));
    Thread thread;
    JMap jmap;
    int thread_id = input.take("thread").toInt();
    QString queryString1 = QString("SELECT * from Threads WHERE id = %1")
            .arg(thread_id);
    if(query.exec(queryString1))
    {
        while(query.next())
        {
            thread.m_nId = thread_id;
            thread.m_bIsClosed = query.value(query.record().indexOf("isClosed")).toBool();
            thread.m_bIsDeleted = query.value(query.record().indexOf("isDeleted")).toBool();
            thread.m_date = query.value(query.record().indexOf("date")).toDateTime();
            thread.m_sCreatorEmail = query.value(query.record().indexOf("creator_email")).toString();
            thread.m_sForum = query.value(query.record().indexOf("forum_short_name")).toString();
            thread.m_sMessage = query.value(query.record().indexOf("message")).toString();
            thread.m_sSlug = query.value(query.record().indexOf("slug")).toString();
            thread.m_sTitle = query.value(query.record().indexOf("title")).toString();
            thread.m_nDislikes = query.value(query.record().indexOf("dislikes")).toInt();
            thread.m_nLikes = query.value(query.record().indexOf("likes")).toInt();
            thread.m_nPoints = thread.m_nLikes - thread.m_nDislikes;
        }
    }
    jmap.insert("date",thread.m_date.toString("yyyy-MM-dd HH:mm:ss"));
    jmap.insert("dislikes", thread.m_nDislikes);
    if(input.contains("forum"))
    {
        ForumDAO fDAO;
        JMap temp;
        temp.insert("short_name", thread.m_sForum);
        temp = fDAO.ForumDetails(temp);
        jmap.insert("forum", temp);
    }
    else
    {
        jmap.insert("forum", thread.m_sForum);
    }
    jmap.insert("id", thread.m_nId);
    jmap.insert("isClosed", thread.m_bIsClosed);
    jmap.insert("isDeleted", thread.m_bIsDeleted);
    jmap.insert("likes", thread.m_nLikes);
    jmap.insert("message", thread.m_sMessage);
    jmap.insert("points", thread.m_nPoints);
    queryString1 = QString("SELECT COUNT(*) FROM Posts WHERE thread_id = %1")
            .arg(thread_id);
    if(query.exec(queryString1))
    {
        if(query.next())
        {
            jmap.insert( "posts", query.value(0).toInt());
        }
    }
    jmap.insert("slug", thread.m_sSlug);
    jmap.insert("title", thread.m_sTitle);
    if(input.contains("user"))
    {
        UserDAO uDAO;
        JMap temp;
        temp = uDAO.UserDetails(thread.m_sCreatorEmail);
        jmap.insert("user", temp);
    }
    else
    {
        jmap.insert("user", thread.m_sCreatorEmail);
    }

    return jmap;
}

JMap ThreadDAO::Close_Open(int nId, int Close)
{
    QString queryString1 = QString("UPDATE Threads SET isClosed = %1 WHERE id = %2")
            .arg(Close)
            .arg(nId);
    QSqlQuery query(QSqlDatabase::database("mysqlcon"));
    JMap jmap;
    if(query.exec(queryString1))
    {
        jmap.insert("thread", nId);
    }
    else
    {
        jmap.insert("error", query.lastError().text());
    }
    return jmap;
}

JMap ThreadDAO::Remove_Restore(int nId, int is_delete)
{
    QString queryString1 = QString("UPDATE Threads SET isDeleted = %1 WHERE id = %2")
            .arg(is_delete)
            .arg(nId);
    QSqlQuery query(QSqlDatabase::database("mysqlcon"));
    JMap jmap;
    if(query.exec(queryString1))
    {
        jmap.insert("thread", nId);
    }
    else
    {
        jmap.insert("error", query.lastError().text());
    }
    queryString1 = QString("UPDATE Posts SET is_deleted_thread = %1 WHERE thread_id = %2")
                .arg(is_delete)
                .arg(nId);
    if(!query.exec(queryString1))
    {
        jmap.insert("error", query.lastError().text());
    }
    return jmap;

}

JMap ThreadDAO::Update(int nId, QString sMessage, QString sSlug)
{
    QString queryString1 = QString("UPDATE Threads SET message = \"%1\", slug = \"%2\" WHERE id = %3")
            .arg(sMessage)
            .arg(sSlug)
            .arg(nId);
    QSqlQuery query(QSqlDatabase::database("mysqlcon"));
    JMap jmap;
    if(query.exec(queryString1))
    {
        jmap.insert("thread", nId);
        return Details(jmap);
    }
    else
    {
        jmap.insert("error", query.lastError().text());
        return jmap;
    }
}


JMap ThreadDAO::Vote(int nId, int vote)
{
    QString queryString1;
    if(vote == 1)
    {
        queryString1 = QString("UPDATE Threads SET likes = likes + 1 WHERE id = %1")
            .arg(nId);
    }
    else
    {
        queryString1 = QString("UPDATE Threads SET dislikes = dislikes + 1 WHERE id = %1")
            .arg(nId);

    }
    QSqlQuery query(QSqlDatabase::database("mysqlcon"));
    JMap jmap;
    if(query.exec(queryString1))
    {
        jmap.insert("thread", nId);
        return Details(jmap);
    }
    else
    {
        jmap.insert("error", query.lastError().text());
        return jmap;
    }
}


QVariantList ThreadDAO::List(JMap input)
{
    QVariantList answer;
    QString order = "";
    QString since = "";
    QString limit = "";
    QSqlError test;
    JMap temp;
    if(input.contains("limit"))
    {
       limit = QString("LIMIT " + input.take("limit").toString());
    }
    if(input.contains("order"))
    {
       order = QString(" ORDER BY date " + input.take("order").toString());
    }
    if(input.contains("since"))
    {
       since =  QString(" AND date > \"" + input.take("since").toString() + "\"");
    }
    QSqlQuery query(QSqlDatabase::database("mysqlcon"));
    if(input.contains("forum"))
    {
        QString queryString1 = QString("SELECT id FROM Threads WHERE forum_short_name = \"%1\" %2 %3 %4")
                .arg(input.take("forum").toString())
                .arg(since)
                .arg(order)
                .arg(limit);
        //qDebug()<<queryString1;
        if(query.exec(queryString1))
        {
            while (query.next())
            {
                temp.insert("thread", query.value(query.record().indexOf("id")).toString());
                answer.push_back(Details(temp));
            }
        }
        else
        {
             test = query.lastError();
        }
        return answer;
    }
    else
    {
        QString queryString1 = QString("SELECT id FROM Threads WHERE creator_email = \"%1\" %2 %3 %4")
                .arg(input.take("user").toString())
                .arg(since)
                .arg(order)
                .arg(limit);
        //qDebug()<<queryString1;
        if(query.exec(queryString1))
        {
            while (query.next())
            {
                temp.insert("thread", query.value(query.record().indexOf("id")).toString());
                answer.push_back(Details(temp));
            }
        }
        else
        {
             test = query.lastError();
        }
        return answer;
    }


}

JMap ThreadDAO::Subscribe(QString sUser, int nThreadId)
{
    QSqlQuery query(QSqlDatabase::database("mysqlcon"));
    QString queryString1 = QString("SELECT deleted FROM subscribe WHERE thread_id = %1 and user_email = \"%2\"")
            .arg(nThreadId)
            .arg(sUser);
    JMap jmap;
    if(query.exec(queryString1))
    {
        if(query.next())
        {
           if(query.value(query.record().indexOf("deleted")).toBool())
           {
               queryString1 = QString("UPDATE subscribe SET deleted = 0 WHERE thread_id = %1 and user_email = \"%2\"")
                       .arg(nThreadId)
                       .arg(sUser);
               if(query.exec(queryString1))
               {
                   jmap.insert("thread", nThreadId);
                   jmap.insert("user", sUser);
                   return jmap;
               }
               else
               {
                   jmap.insert("error", query.lastError().text());
                   return jmap;
               }
           }
           else
           {
               jmap.insert("thread", nThreadId);
               jmap.insert("user", sUser);
               return jmap;
           }

        }
        else
        {
            queryString1 = QString("INSERT INTO subscribe(thread_id, user_email, deleted) VALUES (%1, \"%2\", 0)")
                    .arg(nThreadId)
                    .arg(sUser);
            if(query.exec(queryString1))
            {
                jmap.insert("thread", nThreadId);
                jmap.insert("user", sUser);
                return jmap;
            }
            else
            {
                jmap.insert("error", query.lastError().text());
                return jmap;
            }
        }
    }
    else
    {
        jmap.insert("error", query.lastError().text());
        return jmap;
    }
}



JMap ThreadDAO::UnSubscribe(QString sUser, int nThreadId)
{
    QSqlQuery query(QSqlDatabase::database("mysqlcon"));
    QString queryString1 = QString("UPDATE subscribe SET deleted = 1 WHERE thread_id = %1 and user_email = \"%2\"")
            .arg(nThreadId)
            .arg(sUser);
    JMap jmap;
    if(query.exec(queryString1))
    {
        jmap.insert("thread", nThreadId);
        jmap.insert("user", sUser);
        return jmap;
    }
    else
    {
        jmap.insert("error", query.lastError().text());
        return jmap;
    }
}
