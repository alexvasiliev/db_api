#include "userdao.h"

UserDAO::UserDAO()
{
    /*m_sName = "UserDAO";
    m_pConnection = PDatabaseConnection(new DatabaseConnection(m_sName));*/

    //m_pConnection = DatabaseConnection::GetConnection();
}

UserDAO::~UserDAO()
{

}


JMap     UserDAO::Create(PUser user)
{

    JMap jmap;
    QString queryString1 = QString("INSERT INTO Users(username, about, name, email, isAnonim) VALUES (\"%1\", \"%2\", \"%3\", \"%4\", %5)")
            .arg(user->m_sUserName)
            .arg(user->m_sAbout)
            .arg(user->m_sName)
            .arg(user->m_sEmail)
            .arg(QVariant(QVariant(user->m_bIsAnonim).toInt()).toString());
    QSqlQuery query(QSqlDatabase::database("mysqlcon"));
    if (query.exec(queryString1))
    {
        user->m_nId = query.lastInsertId().toInt();
    }
    else
    {
        QSqlError temp = query.lastError();
        jmap.insert("error" , temp.text());
        return jmap;
    }
    jmap.insert("about", user->m_sAbout);
    jmap.insert("email", user->m_sEmail);
    jmap.insert("id", user->m_nId);
    jmap.insert("isAnonymous", user->m_bIsAnonim);
    jmap.insert("name", user->m_sName);
    jmap.insert("username", user->m_sUserName);
    //QJsonObject answer = QJsonObject::fromVariantMap(jmap);
    return jmap;

}


JMap UserDAO::UserDetails(QString sEmail)
{
    QSqlQuery query(QSqlDatabase::database("mysqlcon"));
    User user;
    JMap jmap;
    QString queryString1 = QString("SELECT * from Users WHERE email = \"%1\"")
            .arg(sEmail);
    if(query.exec(queryString1))
    {
        while(query.next())
        {
            user.m_nId = query.value(query.record().indexOf("id")).toInt();
            user.m_sUserName = query.value(query.record().indexOf("username")).toString();
            user.m_sName = query.value(query.record().indexOf("name")).toString();
            user.m_sAbout = query.value(query.record().indexOf("about")).toString();
            user.m_sEmail = query.value(query.record().indexOf("email")).toString();
            user.m_bIsAnonim = query.value(query.record().indexOf("isAnonim")).toBool();
        }
    }
    jmap.insert("about",user.m_sAbout);
    jmap.insert("email", user.m_sEmail);
    QVariantList follow_email;
    queryString1 = QString("SELECT following_email FROM follows WHERE follow_email = \"%1\" AND deleted = 0")
            .arg(sEmail);
    if (query.exec(queryString1))
    {
        while(query.next())
        {
            follow_email.push_back(query.value(query.record().indexOf("following_email")).toString());
        }
    }
    jmap.insert("followers", follow_email);
    follow_email.clear();

    queryString1 = QString("SELECT follow_email FROM follows WHERE following_email = \"%1\" AND deleted = 0")
            .arg(sEmail);
    if (query.exec(queryString1))
    {
        while(query.next())
        {
            follow_email.push_back(query.value(query.record().indexOf("follow_email")).toString());
        }
    }
    jmap.insert("following", follow_email);
    follow_email.clear();

    jmap.insert("id", user.m_nId);
    jmap.insert("isAnonymous", user.m_bIsAnonim);
    //if(user.m_sName.isEmpty())
      //  jmap.insert("name", QJsonValue::Null);
    //else
        jmap.insert("name", user.m_sName);
    queryString1 = QString("SELECT thread_id FROM subscribe WHERE user_email = \"%1\" AND deleted = 0")
            .arg(sEmail);
    if (query.exec(queryString1))
    {
        while(query.next())
            follow_email.push_back(query.value(query.record().indexOf("thread_id")).toInt());
    }
    jmap.insert("subscriptions", follow_email);
    follow_email.clear();
    //if(user.m_sUserName.isEmpty())
      //  jmap.insert("username", QJsonValue::Null);
    //else
        jmap.insert("username", user.m_sUserName);
    return jmap;
}

JMap UserDAO::UpdateUser(QString sAbout, QString sEmail, QString sName)
{
    QSqlQuery query(QSqlDatabase::database("mysqlcon"));
    QString queryString1 = QString("UPDATE Users SET about =\"%1\", name =\"%2\" WHERE email = \"%3\"")
            .arg(sAbout)
            .arg(sName)
            .arg(sEmail);
    if(query.exec(queryString1))
    {
        return UserDetails(sEmail);
    }else{
        JMap jmap;
        jmap.insert("error", 1);
        return jmap;
    }
}


JMap UserDAO::FollowUser(QString sUserEmail, QString sFollowEmail)
{
    QSqlQuery query(QSqlDatabase::database("mysqlcon"));
    QString queryString1 = QString("SELECT deleted FROM follows WHERE following_email = \"%1\" and follow_email = \"%2\"")
            .arg(sUserEmail)
            .arg(sFollowEmail);
    QSqlError test;
    if(query.exec(queryString1))
    {
        if(query.next())
        {
           if(query.value(query.record().indexOf("deleted")).toBool())
           {
               queryString1 = QString("UPDATE follows SET deleted = 0 WHERE following_email = \"%1\" and follow_email = \"%2\"")
                       .arg(sUserEmail)
                       .arg(sFollowEmail);
               if(!query.exec(queryString1))
               {
                   test = query.lastError();
               }
           }

        }
        else
        {
            queryString1 = QString("INSERT INTO follows(following_email, follow_email, deleted) VALUES (\"%1\", \"%2\", 0)")
                    .arg(sUserEmail)
                    .arg(sFollowEmail);
            if(!query.exec(queryString1))
            {
                test = query.lastError();
            }
        }
    }
    else
    {
        test = query.lastError();
    }
    return UserDetails(sUserEmail);
}

JMap UserDAO::UnFollowUser(QString sUserEmail, QString sFollowEmail)
{
    QSqlQuery query(QSqlDatabase::database("mysqlcon"));
    QString queryString1 = QString("UPDATE follows SET deleted = 1 WHERE following_email = \"%1\" and follow_email = \"%2\"")
            .arg(sUserEmail)
            .arg(sFollowEmail);
    QSqlError test;
    if(!query.exec(queryString1))
    {
        test = query.lastError();
    }
    return UserDetails(sUserEmail);

}


QVariantList  UserDAO::ListFollow(JMap input)
{
    QVariantList answer;
    QString order = "";
    QString where = "";
    QString limit = "";
    QSqlError test;
    if(input.contains("limit"))
    {
        //input.take("limit");
       limit = QString("LIMIT " + input.take("limit").toString());
    }
    if(input.contains("order"))
    {
       order = QString("AND following_email in (SELECT t1.email FROM Users as t1 JOIN Users as t2 on t1.name = t2.name ORDER BY t1.name " + input.take("order").toString() + " )");
    }
    if(input.contains("since_id"))
    {
       where =  QString("AND following_email in (SELECT email from Users where id > " + input.take("since_id").toString() + " )");
    }
    QSqlQuery query(QSqlDatabase::database("mysqlcon"));
    QString queryString1 = QString("SELECT follow_email FROM follows WHERE following_email = \"%1\" AND deleted = 0 %2 %3 %4")
            .arg(input.take("email").toString())
            .arg(where)
            .arg(order)
            .arg(limit);
    if(query.exec(queryString1))
    {
        while (query.next())
        {
            answer.push_back(UserDetails(query.value(query.record().indexOf("follow_email")).toString()));
        }
    }
    else
    {
         test = query.lastError();
    }
    return answer;
}

QVariantList  UserDAO::ListFollowers(JMap input)
{
    QVariantList answer;
    QString order = "";
    QString where = "";
    QString limit = "";
    QSqlError test;
    if(input.contains("limit"))
    {
       limit = QString("LIMIT " + input.take("limit").toString());
    }
    if(input.contains("order"))
    {
       order = QString("AND follow_email in (SELECT t1.email FROM Users as t1 JOIN Users as t2 on t1.name = t2.name ORDER BY t1.name " + input.take("order").toString() + " )");
    }
    if(input.contains("since_id"))
    {
       where =  QString("AND follow_email in (SELECT email from Users where id > " + input.take("since_id").toString() + " )");
    }
    QSqlQuery query(QSqlDatabase::database("mysqlcon"));
    QString queryString1 = QString("SELECT following_email FROM follows WHERE follow_email = \"%1\" %2 %3 %4")
            .arg(input.take("email").toString())
            .arg(where)
            .arg(order)
            .arg(limit);
    if(query.exec(queryString1))
    {
        while (query.next())
        {
            answer.push_back(UserDetails(query.value(query.record().indexOf("following_email")).toString()));
        }
    }
    else
    {
         test = query.lastError();
    }
    return answer;
}

/*QVariantList  UserDAO::ListPosts(JMap input)
{
    QVariantList answer;
    QString order = "";
    QString where = "";
    QString limit = "";
    QSqlError test;
    if(input.contains("limit"))
    {
       limit = QString("LIMIT " + input.take("limit").toString());
    }
    if(input.contains("order"))
    {
       order = QString("AND posts_id in (SELECT t1.email FROM Users as t1 JOIN Users as t2 on t1.name = t2.name ORDER BY t1.name " + input.take("order").toString() + " )");
    }
    if(input.contains("since_id"))
    {
       where =  QString("AND posts_id in (SELECT email from Users where id > " + input.take("since_id").toString() + " )");
    }
    QSqlQuery query(QSqlDatabase::database("mysqlcon"));
    QString queryString1 = QString("SELECT user_email FROM user_follow WHERE follow_email = \"%1\" %2 %3 %4")
            .arg(input.take("email").toString())
            .arg(where)
            .arg(order)
            .arg(limit);
    if(query.exec(queryString1))
    {
        while (query.next())
        {
            answer.push_back(UserDetails(query.value(query.record().indexOf("user_email")).toString()));
        }
    }
    else
    {
         test = query.lastError();
    }
    return answer;
}*/
