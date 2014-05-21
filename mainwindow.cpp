#include "mainwindow.h"
#include <QJsonObject>
#include <QtNetwork>
#include <QDateTime>
#include <QDate>
#include <QByteArray>
#include <QFile>
#include "json.h"

MainWindow::MainWindow()
{
    m_pUserDAO = PUserDAO(new UserDAO());
    m_pForumDAO = PForumDAO(new ForumDAO());
    m_pThreadDAO = PThreadDAO(new ThreadDAO());
    m_pPostDAO = PPostDAO(new PostDAO());
    PDatabaseConnection(new DatabaseConnection("mysqlcon"));

    tcpServer = new QTcpServer(this);
    manager = new QNetworkAccessManager();
    QObject::connect(tcpServer, SIGNAL(newConnection()), this, SLOT(newuser()));

    if (!tcpServer->listen(QHostAddress::Any, 33333)) {
        qDebug() <<  tcpServer->errorString();
    } else {
        qDebug() << tcpServer->isListening() << "TCPSocket listen on port";
    }

    QObject::connect(manager, SIGNAL(finished(QNetworkReply*)),
             this, SLOT(serviceRequestFinished(QNetworkReply*)));

}


MainWindow::~MainWindow()
{
    QSqlDatabase::removeDatabase("mysqlcon");
}


void MainWindow::serviceRequestFinished(QNetworkReply* reply)
{

    delete reply;
}



void MainWindow::newuser()
{
    QTcpSocket* clientSocket=tcpServer->nextPendingConnection();
    int idusersocs=clientSocket->socketDescriptor();
    SClients[idusersocs]=clientSocket;
    QObject::connect(SClients[idusersocs],SIGNAL(readyRead()),this, SLOT(slotReadClient()));
}

void MainWindow::slotReadClient()
{
    QTcpSocket* clientSocket = (QTcpSocket*)sender();
    int idusersocs=clientSocket->socketDescriptor();
    if (clientSocket->canReadLine()) {
        QStringList tokens = QString(clientSocket->readLine()).split(QRegExp("[ \r\n][ \r\n]*"));
        QString test = QString(clientSocket->readAll()).section('{',1,1).prepend('{');
        QByteArray spikeresult;


        QStringList requestList = tokens[1].split("/");
        QtJson::JsonObject  result;
        QtJson::JsonArray   resultArr;
        bool                ArrFlag = 0;
        if (tokens[0] == "GET")
        {
            if(requestList.size() < 4)
            {
            }
            else if (requestList[3] == QString("clear"))
            {
                m_pPostDAO->ClearDatabase();
            }
            else if (requestList[3] == QString("user"))
            {
                if(requestList[4] == QString("listFollowers"))
                {
                    QStringList OptionalList = requestList[5].split("&");
                    JMap input;
                    for(int opt = 0; opt < OptionalList.size(); opt++)
                    {
                         ArrFlag = 1;
                         QString in = OptionalList[opt].replace(QString("%40"),QString("@"));

                         if(in.contains(QString("user")))
                         {
                             input.insert("email", in.section('=',-1));
                         }
                         else if(in.contains(QString("since")))
                         {
                             input.insert("since", in.section('=',-1));
                         }
                         else if(in.contains(QString("limit")))
                         {
                             input.insert("limit", in.section('=',-1));
                         }
                         else if(in.contains(QString("order")))
                         {
                             input.insert("order", in.section('=',-1));
                         }

                    }
                    resultArr = m_pUserDAO->ListFollowers(input);
                }
                else if(requestList[4] == QString("listFollowing"))
                {
                    QStringList OptionalList = requestList[5].split("&");
                    JMap input;
                    for(int opt = 0; opt < OptionalList.size(); opt++)
                    {
                         ArrFlag = 1;
                         QString in = OptionalList[opt].replace(QString("%40"),QString("@"));

                         if(in.contains(QString("user")))
                         {
                             input.insert("email", in.section('=',-1));
                         }
                         else if(in.contains(QString("since")))
                         {
                             input.insert("since", in.section('=',-1));
                         }
                         else if(in.contains(QString("limit")))
                         {
                             input.insert("limit", in.section('=',-1));
                         }
                         else if(in.contains(QString("order")))
                         {
                             input.insert("order", in.section('=',-1));
                         }

                    }
                    resultArr = m_pUserDAO->ListFollow(input);
                }
                else if(requestList[4] == QString("listPosts"))
                {
                    QStringList OptionalList = requestList[5].split("&");
                    JMap input;
                    for(int opt = 0; opt < OptionalList.size(); opt++)
                    {
                         ArrFlag = 1;
                         QString in = OptionalList[opt].replace(QString("%3A"),QString(":")).replace(QString("+"),QString(" ")).replace(QString("%40"),QString("@"));

                         if(in.contains(QString("user")))
                         {
                             input.insert("user", in.section('=',-1));
                         }
                         else if(in.contains(QString("since")))
                         {
                             input.insert("since", in.section('=',-1));
                         }
                         else if(in.contains(QString("limit")))
                         {
                             input.insert("limit", in.section('=',-1));
                         }
                         else if(in.contains(QString("order")))
                         {
                             input.insert("order", in.section('=',-1));
                         }

                    }
                    resultArr = m_pPostDAO->UserListPosts(input);
                }
                else if(requestList[4] == QString("details"))
                {
                    QString email = requestList[5].replace(QString("%40"),QString("@")).section('=',1,1);
                    result = m_pUserDAO->UserDetails(email);
                }
            }
            else if(requestList[3] == QString("forum"))
            {
                if(requestList[4] == QString("details"))
                {
                    JMap input;
                    if(requestList[5].contains("user"))
                    {
                        input.insert("user","user");
                    }
                    input.insert("short_name", requestList[5].section("forum=",1,1));
                    result = m_pForumDAO->ForumDetails(input);
                }
                else if(requestList[4] == QString("listPosts"))
                {
                   QStringList OptionalList = requestList[5].split("&");
                   JMap input;
                   for(int opt = 0; opt < OptionalList.size(); opt++)
                   {
                        ArrFlag = 1;
                        QString in = OptionalList[opt].replace(QString("%3A"),QString(":")).replace(QString("+"),QString(" "));//.section('=',-1);

                        if(in.contains(QString("thread")))
                        {
                            input.insert("thread", "thread");
                        }
                        if(in.contains(QString("forum")))
                        {
                            input.insert("RelForum", "forum");
                        }
                        if(in.contains(QString("user")))
                        {
                            input.insert("user", "user");
                        }
                        else if(in.contains(QString("since")))
                        {
                            input.insert("since", in.section('=',-1));
                        }
                        else if(in.contains(QString("limit")))
                        {
                            input.insert("limit", in.section('=',-1));
                        }
                        else if(in.contains(QString("order")))
                        {
                            input.insert("order", in.section('=',-1));
                        }
                        else if(in.contains(QString("forum")))
                        {
                            input.insert("forum", in.section('=',-1));
                        }

                   }
                   resultArr = m_pPostDAO->ForumListPosts(input);
                }
                else if(requestList[4] == QString("listThreads"))
                {
                    QStringList OptionalList = requestList[5].split("&");
                    JMap input;
                    for(int opt = 0; opt < OptionalList.size(); opt++)
                    {
                         ArrFlag = 1;
                         QString in = OptionalList[opt].replace(QString("%3A"),QString(":")).replace(QString("+"),QString(" "));
                         if(in.contains(QString("forum")))
                         {
                             input.insert("RelForum", "forum");
                         }
                         if(in.contains(QString("user")))
                         {
                             input.insert("user", "user");
                         }
                         else if(in.contains(QString("since")))
                         {
                             input.insert("since", in.section('=',-1));
                         }
                         else if(in.contains(QString("limit")))
                         {
                             input.insert("limit", in.section('=',-1));
                         }
                         else if(in.contains(QString("order")))
                         {
                             input.insert("order", in.section('=',-1));
                         }
                         else if(in.contains(QString("forum")))
                         {
                             input.insert("forum", in.section('=',-1));
                         }

                    }
                    resultArr = m_pPostDAO->ForumListThreads(input);



                }
                else if(requestList[4] == QString("listUsers"))
                {
                    QStringList OptionalList = requestList[5].split("&");
                    JMap input;
                    for(int opt = 0; opt < OptionalList.size(); opt++)
                    {
                         ArrFlag = 1;
                         QString in = OptionalList[opt].replace(QString("%3A"),QString(":")).replace(QString("+"),QString(" "));
                         if(in.contains(QString("since")))
                         {
                             input.insert("since", in.section('=',-1));
                         }
                         else if(in.contains(QString("limit")))
                         {
                             input.insert("limit", in.section('=',-1));
                         }
                         else if(in.contains(QString("order")))
                         {
                             input.insert("order", in.section('=',-1));
                         }
                         else if(in.contains(QString("forum")))
                         {
                             input.insert("forum", in.section('=',-1));
                         }

                    }
                    resultArr = m_pPostDAO->ForumListUsers(input);

                }
            }
            else if(requestList[3] == QString("post"))
            {
                if(requestList[4] == QString("details"))
                {
                    JMap input;
                    if(requestList[5].contains("user"))
                    {
                        input.insert("user","user");
                    }
                    if(requestList[5].contains("forum"))
                    {
                        input.insert("forum","forum");
                    }
                    if(requestList[5].contains("thread"))
                    {
                        input.insert("thread","thread");
                    }
                    int n = requestList[5].section("post=",-1).indexOf('&');
                    input.insert("id", requestList[5].section("post=",-1).left(n));
                    result = m_pPostDAO->PostDetails(input);
                }
                else if(requestList[4] == QString("list"))
                {
                    QStringList OptionalList = requestList[5].split("&");
                    JMap input;
                    for(int opt = 0; opt < OptionalList.size(); opt++)
                    {
                         ArrFlag = 1;
                         QString in = OptionalList[opt].replace(QString("%3A"),QString(":")).replace(QString("+"),QString(" "));
                         if(in.contains(QString("since")))
                         {
                             input.insert("since", in.section('=',-1));
                         }
                         else if(in.contains(QString("limit")))
                         {
                             input.insert("limit", in.section('=',-1));
                         }
                         else if(in.contains(QString("order")))
                         {
                             input.insert("order", in.section('=',-1));
                         }
                         else if(in.contains(QString("forum")))
                         {
                             input.insert("forum", in.section('=',-1));
                         }
                         else if(in.contains(QString("thread")))
                         {
                             input.insert("thread", in.section('=',-1));
                         }

                    }
                    resultArr = m_pPostDAO->List(input);
                }

            }

            else if (requestList[3] == QString("thread"))
            {
                if(requestList[4] == QString("details"))
                {
                    JMap input;
                    if(requestList[5].contains("user"))
                    {
                        input.insert("user","user");
                    }
                    if(requestList[5].contains("forum"))
                    {
                        input.insert("forum","forum");
                    }
                    input.insert("thread", requestList[5].section("thread=",1,1));
                    result = m_pThreadDAO->Details(input);

                }
                else if(requestList[4] == QString("list"))
                {

                    QStringList OptionalList = requestList[5].split("&");
                    JMap input;
                    for(int opt = 0; opt < OptionalList.size(); opt++)
                    {
                         ArrFlag = 1;
                         QString in = OptionalList[opt].replace(QString("%3A"),QString(":")).replace(QString("+"),QString(" ")).replace(QString("%40"),QString("@"));
                         if(in.contains(QString("since")))
                         {
                             input.insert("since", in.section('=',-1));
                         }
                         else if(in.contains(QString("limit")))
                         {
                             input.insert("limit", in.section('=',-1));
                         }
                         else if(in.contains(QString("order")))
                         {
                             input.insert("order", in.section('=',-1));
                         }
                         else if(in.contains(QString("forum")))
                         {
                             input.insert("forum", in.section('=',-1));
                         }
                         else if(in.contains(QString("user")))
                         {
                             input.insert("user", in.section('=',-1));
                         }

                    }
                    resultArr = m_pThreadDAO->List(input);

                }
                else if(requestList[4] == QString("listPosts"))
                {

                    QStringList OptionalList = requestList[5].split("&");
                    JMap input;
                    for(int opt = 0; opt < OptionalList.size(); opt++)
                    {
                         ArrFlag = 1;
                         QString in = OptionalList[opt].replace(QString("%3A"),QString(":")).replace(QString("+"),QString(" "));
                         if(in.contains(QString("since")))
                         {
                             input.insert("since", in.section('=',-1));
                         }
                         else if(in.contains(QString("limit")))
                         {
                             input.insert("limit", in.section('=',-1));
                         }
                         else if(in.contains(QString("order")))
                         {
                             input.insert("order", in.section('=',-1));
                         }
                         else if(in.contains(QString("thread")))
                         {
                             input.insert("thread", in.section('=',-1));
                         }

                    }
                    resultArr = m_pPostDAO->ThreadsListPosts(input);
                }
            }
        }
        else if(tokens[0] == "POST")
        result = QtJson::parse(test).toMap();
        {
            if(requestList.size() < 4)
            {
            }
            else if (requestList[3] == QString("clear"))
            {
                m_pPostDAO->ClearDatabase();
            }
            else if (requestList[3] == QString("user"))
            {

                if(requestList[4] == QString("create"))
                {
                    PUser user = PUser(new User(result.take("name").toString(), result.take("username").toString(), result.take("email").toString(),result.take("about").toString() ,result.take("isAnonymous").toInt()));
                    result = m_pUserDAO->Create(user);
                }
                else if(requestList[4] == QString("follow"))
                {
                    result = m_pUserDAO->FollowUser(result.take("follower").toString(), result.take("followee").toString());
                }         
                else if(requestList[4] == QString("unfollow"))
                {
                    result = m_pUserDAO->UnFollowUser(result.take("follower").toString(), result.take("followee").toString());
                }
                else if(requestList[4] == QString("updateProfile"))
                {
                    result = m_pUserDAO->UpdateUser(result.take("about").toString(), result.take("user").toString(), result.take("name").toString());
                }

            }
            else if(requestList[3] == QString("forum"))
            {

                if(requestList[4] == QString("create"))
                {
                    PForum forum = PForum(new Forum(result.take("name").toString(), result.take("short_name").toString(), result.take("user").toString()));
                    result = m_pForumDAO->Create(forum);
                }

            }
            else if(requestList[3] == QString("post"))
            {

                if(requestList[4] == QString("create"))
                {
                    QDateTime dateTime = QDateTime::fromString(result.take("date").toString(), "yyyy-MM-dd HH:mm:ss");
                    PPost post = PPost(new Post(dateTime, result.take("message").toString(), result.take("user").toString(), result.take("thread").toInt(),
                                                result.take("forum").toString(), result.take("parent").toInt(), result.take("isApproved").toBool(),
                                                result.take("isHighlighted").toBool(), result.take("isEdited").toBool(), result.take("isSpam").toBool(),
                                                result.take("isDeleted").toBool()));
                    result = m_pPostDAO->Create(post);
                }
                else if(requestList[4] == QString("remove"))
                {
                    result = m_pPostDAO->Remove_Restore(result.take("post").toInt(), 1);
                }
                else if(requestList[4] == QString("restore"))
                {
                    result = m_pPostDAO->Remove_Restore(result.take("post").toInt(), 0);
                }
                else if(requestList[4] == QString("update"))
                {
                    result = m_pPostDAO->Update(result.take("post").toInt(),result.take("message").toString());
                }
                else if(requestList[4] == QString("vote"))
                {
                    result = m_pPostDAO->Vote(result.take("post").toInt(), result.take("vote").toInt());
                }

            }
            else if (requestList[3] == QString("thread"))
            {

                if(requestList[4] == QString("create"))
                {
                    QDateTime dateTime = QDateTime::fromString(result.take("date").toString(), "yyyy-MM-dd HH:mm:ss");
                    PThread thread = PThread(new Thread(result.take("title").toString(), dateTime, result.take("message").toString(),
                                                        result.take("slug").toString(), result.take("forum").toString(), result.take("user").toString(),
                                                        result.take("isClosed").toBool(), result.take("isDeleted").toBool()));
                    result = m_pThreadDAO->Create(thread);
                }
                else if(requestList[4] == QString("close"))
                {
                    result = m_pThreadDAO->Close_Open(result.take("thread").toInt(), 1);
                }
                else if(requestList[4] == QString("open"))
                {
                    result = m_pThreadDAO->Close_Open(result.take("thread").toInt(), 0);

                }
                else if(requestList[4] == QString("remove"))
                {
                    result = m_pThreadDAO->Remove_Restore(result.take("thread").toInt(), 1);
                }
                else if(requestList[4] == QString("restore"))
                {
                    result = m_pThreadDAO->Remove_Restore(result.take("thread").toInt(), 0);
                }
                else if(requestList[4] == QString("subscribe"))
                {
                    result = m_pThreadDAO->Subscribe(result.take("user").toString(),result.take("thread").toInt());
                }
                else if(requestList[4] == QString("unsubscribe"))
                {
                    result = m_pThreadDAO->UnSubscribe(result.take("user").toString(),result.take("thread").toInt());
                }
                else if(requestList[4] == QString("update"))
                {
                    result = m_pThreadDAO->Update(result.take("thread").toInt(), result.take("message").toString(), result.take("slug").toString());
                }
                else if(requestList[4] == QString("vote"))
                {
                    result = m_pThreadDAO->Vote(result.take("thread").toInt(), result.take("vote").toInt());
                }

            }
            if(ArrFlag){
                bool spike = 0;
                if(resultArr.empty())
                    spike = 1;
                spikeresult = QtJson::serialize(resultArr);
                spikeresult.replace(QByteArray("\"\""), QByteArray("null"));
                if(spike == 1)
                    spikeresult.replace(QByteArray("[  ]"), QByteArray("{\"response\": [  ]} "));
                clientSocket->write(spikeresult);
            }
            else
            {
                spikeresult = QtJson::serialize(result);
                spikeresult.replace(QByteArray("\"\""), QByteArray("null"));
                clientSocket->write(spikeresult);
            }
            //qDebug() << spikeresult;
        }
    }

    // Если нужно закрыть сокет
    clientSocket->close();
    SClients.remove(idusersocs);
}

