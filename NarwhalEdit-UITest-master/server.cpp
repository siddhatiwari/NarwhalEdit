#include "server.h"
#include <QDebug>
#include <QApplication>
#include "mythread.h"

Server::Server(QObject *parent) : QTcpServer(parent)
{
    connect(this, SIGNAL(newConnection()), this, SLOT(newConnection()));
}

Server::~Server()
{
    close();
}

void Server::startServer()
{
    if (!listen(QHostAddress::Any, 2000))
        qDebug() << "Could not start server";
    else
        qDebug() << "Listening...";
}

void Server::newConnection()
{
    while (hasPendingConnections()) {
        qDebug() << "Connecting...";

        QTcpSocket *socket = nextPendingConnection();
        connect(socket, SIGNAL(readyRead()), SLOT(readyRead()));
        connect(socket, SIGNAL(disconnected()), SLOT(disconnected()));

        sockets.push_back(socket);
    }
}

void Server::disconnected()
{
    QTcpSocket *socket = static_cast<QTcpSocket *>(sender());
    socket->deleteLater();
}

void Server::readyRead()
{
    QTcpSocket *socket = static_cast<QTcpSocket *>(sender());
    QByteArray data  = socket->readAll();
    emit dataReceived(data);
}

