#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtNetwork>
#include <QTcpSocket>
#include <QObject>
#include <QByteArray>
#include <QDebug>
#include <userdao.h>
#include <postdao.h>
#include <forumdao.h>
#include <threaddao.h>


class QTcpServer;

class MainWindow : public QObject
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

private slots:
    void on_starting_clicked();
    void on_stoping_clicked();
    void newuser();
    void slotReadClient();
    void serviceRequestFinished(QNetworkReply* reply);

private:
    QTcpServer *tcpServer;
    int server_status;
    QMap<int,QTcpSocket *> SClients;
    QNetworkAccessManager *manager;

    PUserDAO m_pUserDAO;
    PForumDAO m_pForumDAO;
    PThreadDAO m_pThreadDAO;
    PPostDAO m_pPostDAO;
};

#endif // MAINWINDOW_H
