#include "mytcpserver.h"
#include "functions.h"
#include <QDebug>
#include <QCoreApplication>

MyTcpServer::~MyTcpServer()
{
    //mTcpSocket->close();
    mTcpServer->close();
    server_status=0;
}
MyTcpServer::MyTcpServer(QObject *parent) : QObject(parent){
    mTcpServer = new QTcpServer(this);
    connect(mTcpServer, &QTcpServer::newConnection,
            this, &MyTcpServer::slotNewConnection);

    if(!mTcpServer->listen(QHostAddress::Any, 33333)){
        qDebug() << "server is not started";
    } else {
        server_status=1;
        qDebug() << "server is started";
    }
}

void MyTcpServer::slotNewConnection(){
    if(server_status==1)
    {
        QTcpSocket *curr_mTcpSocket;
        curr_mTcpSocket = mTcpServer->nextPendingConnection();
        curr_mTcpSocket->write("Hello, World!!! I am echo server!\r\n");
        connect(curr_mTcpSocket, &QTcpSocket::readyRead,
                this,&MyTcpServer::slotServerRead);
        connect(curr_mTcpSocket,&QTcpSocket::disconnected,
                this,&MyTcpServer::slotClientDisconnected);
        qDebug() << "client connected" << curr_mTcpSocket->socketDescriptor();
        mTcpSocket[curr_mTcpSocket->socketDescriptor()] = curr_mTcpSocket;
    }
}

void MyTcpServer::slotServerRead()
{
    QByteArray array;
    QTcpSocket *curr_mTcpSocket = (QTcpSocket*)sender();
    while(curr_mTcpSocket->bytesAvailable()>0)
    {
        array.append(curr_mTcpSocket->readAll());
    }
    if (array.right(1) == "\n")
    {
        curr_mTcpSocket->write(parse(array));
    }
}

void MyTcpServer::slotClientDisconnected(){
    QTcpSocket *curr_mTcpSocket = (QTcpSocket*)sender();
    curr_mTcpSocket->close();
}


/*
void MyTcpServer::slotServerRead()
{
    QByteArray array;
    while(mTcpSocket->bytesAvailable()>0)
    {
        array.append(mTcpSocket->readAll());
    }
    if (array.right(1) == "\n")
    {
        mTcpSocket->write(parse(array));
    }
}
*/
