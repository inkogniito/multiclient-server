#include "mytcpserver.h"
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
    QTcpSocket *curr_mTcpSocket = mTcpSocket[mTcpServer->socketDescriptor()];
    while(curr_mTcpSocket->bytesAvailable()>0)
    {
        QByteArray array =curr_mTcpSocket->readAll();
        curr_mTcpSocket->write(array);
    }
}

void MyTcpServer::slotClientDisconnected(){
    QTcpSocket *curr_mTcpSocket = mTcpSocket[mTcpServer->socketDescriptor()];
    curr_mTcpSocket->close();
}
