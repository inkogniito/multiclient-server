#include "mytcpserver.h"
#include "functions.h"
#include <QDebug>
#include <QCoreApplication>

MyTcpServer::~MyTcpServer()
{
    mTcpServer->close();
    server_status=0;
}
MyTcpServer::MyTcpServer(QObject *parent) : QObject(parent)
{
    mTcpServer = new QTcpServer(this);
    connect(mTcpServer, &QTcpServer::newConnection,
            this, &MyTcpServer::slotNewConnection);

    if(!mTcpServer->listen(QHostAddress::Any, 33333))
    {
        qDebug() << "server is not started";
    }
    else
    {
        server_status=1;
        qDebug() << "server is started";
    }
}

void MyTcpServer::slotNewConnection()
{
    if(server_status==1)
    {
        QTcpSocket *curr_mTcpSocket;
        curr_mTcpSocket = mTcpServer->nextPendingConnection();
        curr_mTcpSocket->write("Авторизуйтесь или зарегистрируйтесь чтобы получить доступ к полному функционалу.\r\n");
        connect(curr_mTcpSocket, &QTcpSocket::readyRead,
                this,&MyTcpServer::slotServerRead);
        connect(curr_mTcpSocket,&QTcpSocket::disconnected,
                this,&MyTcpServer::slotClientDisconnected);
        qDebug() << "client connected" << curr_mTcpSocket->socketDescriptor();
        mTcpSocket[curr_mTcpSocket->socketDescriptor()] = curr_mTcpSocket;
        authstat.insert(curr_mTcpSocket->socketDescriptor(), 0);
    }
}

void MyTcpServer::slotServerRead()
{
    QByteArray array;
    QTcpSocket *curr_mTcpSocket = (QTcpSocket*)sender();
    QStringList arr;
    QString arval;
    while(curr_mTcpSocket->bytesAvailable()>0)
    {
        array.append(curr_mTcpSocket->readAll());
    }

    if (array.right(1) == "\n")
    {
        arval = QString::fromUtf8(array);
        arr = arval.left(arval.length() - 2).split(" ");

        if (authstat.value(curr_mTcpSocket->socketDescriptor()) == 1)
        {
            if (arr.size() == 1 and arr[0] == "logout")
            {
                curr_mTcpSocket->write(parse(array, authstat.value(curr_mTcpSocket->socketDescriptor())));
                authstat[curr_mTcpSocket->socketDescriptor()] = 0;
            }
            else
            {
                curr_mTcpSocket->write(parse(array, authstat.value(curr_mTcpSocket->socketDescriptor())));
            }

        }
        else
        {
            if (arr[0]=="reg" or arr[0]=="login")
            {
                QByteArray flag = parse(array, authstat.value(curr_mTcpSocket->socketDescriptor()));
                if (flag=="Авторизация успешна!\n" or flag=="Регистрация успешна!\n")
                {
                    authstat[curr_mTcpSocket->socketDescriptor()] = 1;
                }
                curr_mTcpSocket->write(flag);
            }
            else
            {
                curr_mTcpSocket->write("Авторизуйтесь или зарегистрируйтесь чтобы получить доступ к полному функционалу.\r\n");
            }
        }

    }
}

void MyTcpServer::slotClientDisconnected()
{
    QTcpSocket *curr_mTcpSocket = (QTcpSocket*)sender();
    authstat.remove(curr_mTcpSocket->socketDescriptor());
    curr_mTcpSocket->close();
}
