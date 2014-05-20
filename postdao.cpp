#include "postdao.h"
#include "forumdao.h"
#include "threaddao.h"
#include "userdao.h"


PostDAO::PostDAO()
{

}

JMap PostDAO::Create(PPost post)
{
    JMap jmap;
    if(post->m_nParentId == 0)
    {
        post->m_nParentId = -1;
    }
    QString queryString1 = QString("INSERT INTO Posts(date, message, creator_email, thread_id, forum, \
                                   parent_id, is_aproved, is_highlighted, is_edited, is_spam, is_deleted)\
            VALUES (\"%1\", \"%2\", \"%3\", %4, \"%5\", %6, %7, %8, %9, %10, %11)")
            .arg(post->m_date.toString("yyyy-MM-dd HH:mm:ss"))
            .arg(post->m_sMessage)
            .arg(post->m_sCreatorEmail)
            .arg(post->m_nTHreadId)
            .arg(post->m_sForum)
            .arg(post->m_nParentId)
            .arg(QVariant(post->m_bIsAproved).toInt())
            .arg(QVariant(post->m_bIsHighlighted).toInt())
            .arg(QVariant(post->m_bIsEdited).toInt())
            .arg(QVariant(post->m_bIsSpam).toInt())
            .arg(QVariant(post->m_bIsDeleted).toInt());
    QSqlQuery query(QSqlDatabase::database("mysqlcon"));
    if (query.exec(queryString1))
        post->m_nId = query.lastInsertId().toInt();
    else
    {
        QSqlError temp = query.lastError();
        jmap.insert("error" , temp.text());
        return jmap;
    }
    jmap.insert("date", post->m_date.toString("YYYY-MM-DD hh-mm-ss"));
    jmap.insert("forum", post->m_sForum);
    jmap.insert("id", post->m_nId);
    jmap.insert("isApproved", post->m_bIsAproved);
    jmap.insert("isDeleted", post->m_bIsDeleted);
    jmap.insert("isEdited", post->m_bIsEdited);
    jmap.insert("isHighlighted", post->m_bIsHighlighted);
    jmap.insert("isSpam", post->m_bIsSpam);
    jmap.insert("message", post->m_sMessage);
    jmap.insert("thread", post->m_nTHreadId);
    jmap.insert("user", post->m_sCreatorEmail);


    return jmap;
}


JMap PostDAO::PostDetails (JMap input)
{
    QSqlQuery query(QSqlDatabase::database("mysqlcon"));
    Post post;
    JMap jmap;
    int id = input.take("id").toInt();
    QString queryString1 = QString("SELECT * from Posts WHERE id = %1")
            .arg(id);
    if(query.exec(queryString1))
    {
        while(query.next())
        {
            post.m_date = query.value(query.record().indexOf("date")).toDateTime();
            post.m_sMessage = query.value(query.record().indexOf("message")).toString();
            post.m_sCreatorEmail = query.value(query.record().indexOf("creator_email")).toString();
            post.m_nTHreadId = query.value(query.record().indexOf("thread_id")).toInt();
            post.m_sForum = query.value(query.record().indexOf("forum")).toString();
            post.m_nDisLikes = query.value(query.record().indexOf("dislikes")).toInt();
            post.m_nLikes = query.value(query.record().indexOf("likes")).toInt();
            post.m_bIsAproved = query.value(query.record().indexOf("is_aproved")).toBool();
            post.m_bIsDeleted = query.value(query.record().indexOf("is_deleted")).toBool();
            post.m_bIsDeletedParent = query.value(query.record().indexOf("is_deleted_parent")).toBool();
            post.m_bIsDeletedThread = query.value(query.record().indexOf("is_deleted_thread")).toBool();
            post.m_bIsEdited = query.value(query.record().indexOf("is_edited")).toBool();
            post.m_bIsHighlighted = query.value(query.record().indexOf("is_highlighted")).toBool();
            post.m_bIsSpam = query.value(query.record().indexOf("is_spam")).toBool();
            post.m_nParentId = query.value(query.record().indexOf("parent_id")).toInt();
            post.m_nPoints = post.m_nLikes - post.m_nDisLikes;
        }
    }
    jmap.insert("date",post.m_date.toString("yyyy-MM-dd HH:mm:ss"));
    jmap.insert("dislikes", post.m_nDisLikes);
    if(input.contains("forum"))
    {
        ForumDAO fDAO;
        JMap temp;
        temp.insert("short_name", post.m_sForum);
        temp = fDAO.ForumDetails(temp);
        jmap.insert("forum", temp);
    }
    else
    {
        jmap.insert("forum",post.m_sForum);
    }
    jmap.insert("id", id);
    jmap.insert("isApproved", post.m_bIsAproved);
    if(!post.m_bIsDeletedParent && !post.m_bIsDeletedThread)
        jmap.insert("isDeleted", post.m_bIsDeleted);
    else
        jmap.insert("isDeleted", true);
    jmap.insert("isEdited", post.m_bIsEdited);
    jmap.insert("isHighlighted", post.m_bIsHighlighted);
    jmap.insert("isSpam", post.m_bIsSpam);
    jmap.insert("likes", post.m_nLikes);
    jmap.insert("message", post.m_sMessage);
    if(post.m_nParentId == -1)
    {
        jmap.insert("parent", "");
    }
    else
    {
        jmap.insert("parent", post.m_nParentId);
    }
    jmap.insert("points", post.m_nPoints);
    if(input.contains("thread"))
    {
        ThreadDAO tDAO;
        JMap temp;
        temp.insert("thread", post.m_nTHreadId);
        temp = tDAO.Details(temp);
        jmap.insert("thread", temp);

    }
    else
    {
        jmap.insert("thread", post.m_nTHreadId);
    }
    if(input.contains("user"))
    {
        UserDAO uDAO;
        JMap temp;
        temp = uDAO.UserDetails(post.m_sCreatorEmail);
        jmap.insert("user", temp);
    }
    else
    {
        jmap.insert("user", post.m_sCreatorEmail);
    }
    return jmap;
}



QVariantList  PostDAO::ForumListPosts(JMap input)
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
    QString queryString1 = QString("SELECT id FROM Posts WHERE forum = \"%1\" %2 %3 %4")
            .arg(input.take("forum").toString())
            .arg(since)
            .arg(order)
            .arg(limit);
    if(query.exec(queryString1))
    {
        while (query.next())
        {
            temp.insert("id", query.value(query.record().indexOf("id")).toString());

            if(input.contains(QString("thread")))
            {
                temp.insert("thread", "thread");
            }
            if(input.contains(QString("RelForum")))
            {
                temp.insert("forum", "forum");
            }
            if(input.contains(QString("user")))
            {
                temp.insert("user", "user");
            }
            answer.push_back(PostDetails(temp));
        }
    }
    else
    {
         test = query.lastError();
    }
    return answer;
}


QVariantList PostDAO::ForumListThreads(JMap input)
{
    QVariantList answer;
    QString order = "";
    QString since = "";
    QString limit = "";
    QSqlError test;
    JMap temp;
    ThreadDAO tDAO;
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
    QString queryString1 = QString("SELECT id FROM Threads WHERE forum_short_name = \"%1\" %2 %3 %4")
            .arg(input.take("forum").toString())
            .arg(since)
            .arg(order)
            .arg(limit);
    if(query.exec(queryString1))
    {
        while (query.next())
        {
            temp.insert("thread", query.value(query.record().indexOf("id")).toString());
            if(input.contains(QString("RelForum")))
            {
                temp.insert("forum", "forum");
            }
            if(input.contains(QString("user")))
            {
                temp.insert("user", "user");
            }
            answer.push_back(tDAO.Details(temp));
        }
    }
    else
    {
         test = query.lastError();
    }
    return answer;
}

QVariantList PostDAO::ForumListUsers(JMap input)
{
    QVariantList answer;
    QString order = "";
    QString since = "";
    QString limit = "";
    QSqlError test;
    UserDAO uDAO;
    if(input.contains("limit"))
    {
       limit = QString("LIMIT " + input.take("limit").toString());
    }
    if(input.contains("order"))
    {
       order = QString(" ORDER BY t2.id " + input.take("order").toString());
    }
    if(input.contains("since"))
    {
       since =  QString(" AND t2.id > " + input.take("since").toString());
    }
    QSqlQuery query(QSqlDatabase::database("mysqlcon"));
    QString queryString1 = QString("SELECT DISTINCT creator_email FROM Posts as t1 JOIN Users as t2 on t1.creator_email = t2.email\
                                    WHERE t1.forum = \"%1\" %2 %3 %4 ")
                .arg(input.take("forum").toString())
                .arg(since)
                .arg(order)
                .arg(limit);
    /*QString queryString1 = QString("SELECT DISTINCT creator_email FROM Posts WHERE forum = \"%1\" AND creator_email in (SELECT email as FROM Users %2 %3) %4 ")
            .arg(input.take("forum").toString())
            .arg(since)
            .arg(order)
            .arg(limit);*/
    if(query.exec(queryString1))
    {
        while (query.next())
        {
            answer.push_back(uDAO.UserDetails(query.value(query.record().indexOf("creator_email")).toString()));
        }
    }
    else
    {
         test = query.lastError();
    }
    return answer;
}

QVariantList PostDAO::List(JMap input)
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
        QString queryString1 = QString("SELECT id FROM Posts WHERE forum = \"%1\" %2 %3 %4")
                .arg(input.take("forum").toString())
                .arg(since)
                .arg(order)
                .arg(limit);
        if(query.exec(queryString1))
        {
            while (query.next())
            {
                temp.insert("id", query.value(query.record().indexOf("id")).toString());
                answer.push_back(PostDetails(temp));
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
        QString queryString1 = QString("SELECT id FROM Posts WHERE thread_id = %1 %2 %3 %4")
                .arg(input.take("thread").toString())
                .arg(since)
                .arg(order)
                .arg(limit);
        if(query.exec(queryString1))
        {
            while (query.next())
            {
                temp.insert("id", query.value(query.record().indexOf("id")).toString());
                answer.push_back(PostDetails(temp));
            }
        }
        else
        {
             test = query.lastError();
        }
        return answer;
    }

}

JMap PostDAO::Remove_Restore(int nId, int is_delete)
{
    QString queryString1 = QString("UPDATE Posts SET is_deleted = %1 WHERE id = %2")
            .arg(is_delete)
            .arg(nId);
    QSqlQuery query(QSqlDatabase::database("mysqlcon"));
    JMap jmap;
    if(query.exec(queryString1))
    {
        jmap.insert("post", nId);
    }
    else
    {
        jmap.insert("error", query.lastError().text());
    }
    queryString1 = QString("UPDATE Posts SET is_deleted_parent = %1 WHERE parent_id = %2")
                .arg(is_delete)
                .arg(nId);
    if(!query.exec(queryString1))
    {
        jmap.insert("error", query.lastError().text());
    }
    return jmap;

}

JMap PostDAO::Update (int nId, QString sMessage)
{
    QString queryString1 = QString("UPDATE Posts SET message = \"%1\" WHERE id = %2")
            .arg(sMessage)
            .arg(nId);
    QSqlQuery query(QSqlDatabase::database("mysqlcon"));
    JMap jmap;
    if(query.exec(queryString1))
    {
        jmap.insert("id", nId);
        return PostDetails(jmap);
    }
    else
    {
        jmap.insert("error", query.lastError().text());
        return jmap;
    }
}

JMap PostDAO::Vote(int nId, int vote)
{
    QString queryString1;
    if(vote == 1)
    {
        queryString1 = QString("UPDATE Posts SET likes = likes + 1 WHERE id = %1")
            .arg(nId);
    }
    else
    {
        queryString1 = QString("UPDATE Posts SET dislikes = dislikes + 1 WHERE id = %1")
            .arg(nId);

    }
    QSqlQuery query(QSqlDatabase::database("mysqlcon"));
    JMap jmap;
    if(query.exec(queryString1))
    {
        jmap.insert("id", nId);
        return PostDetails(jmap);
    }
    else
    {
        jmap.insert("error", query.lastError().text());
        return jmap;
    }
}

QVariantList PostDAO::ThreadsListPosts(JMap input)
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
    QString queryString1 = QString("SELECT id FROM Posts WHERE thread_id = \"%1\" %2 %3 %4")
            .arg(input.take("thread").toString())
            .arg(since)
            .arg(order)
            .arg(limit);
    //qDebug()<<queryString1;
    if(query.exec(queryString1))
    {
        while (query.next())
        {
            temp.insert("id", query.value(query.record().indexOf("id")).toString());
            answer.push_back(PostDetails(temp));
        }
    }
    else
    {
         test = query.lastError();
    }
    return answer;

}

QVariantList PostDAO::UserListPosts(JMap input)
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
    QString queryString1 = QString("SELECT id FROM Posts WHERE creator_email = \"%1\" %2 %3 %4")
            .arg(input.take("user").toString())
            .arg(since)
            .arg(order)
            .arg(limit);
    if(query.exec(queryString1))
    {
        while (query.next())
        {
            temp.insert("id", query.value(query.record().indexOf("id")).toString());
            answer.push_back(PostDetails(temp));
        }
    }
    else
    {
         test = query.lastError();
    }
    return answer;
}

void PostDAO::ClearDatabase()
{
    QSqlQuery query(QSqlDatabase::database("mysqlcon"));
    QSqlError temp;
    if(!query.exec("delete from Posts; delete from subscribe; delete from follows; delete from Threads; delete from Forums; delete from Users;")){
        temp = query.lastError();
    }

}
