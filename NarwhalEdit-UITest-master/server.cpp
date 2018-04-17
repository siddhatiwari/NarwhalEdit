
#include "server.h"
#include <QDebug>
#include <QApplication>

Server::Server(QObject *parent) : QTcpServer(parent)
{
    connect(this, SIGNAL(newConnection()), this, SLOT(newConnection()));
}

Server::~Server()
{
    close();
}

void Server::startServer(int port)
{
    if (!listen(QHostAddress::Any, port))
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
        connections++;
    }
}

void Server::disconnected()
{
    QTcpSocket *socket = static_cast<QTcpSocket *>(sender());
    for (int i = 0; i < sockets.size(); i++) {
        if (sockets.at(i) == socket)
            sockets.erase(sockets.begin() + i);
    }

    socket->deleteLater();
    connections--;
}

void Server::readyRead()
{
    QTcpSocket *socket = static_cast<QTcpSocket *>(sender());
    QByteArray data = socket->readAll();
    emit dataReceived(data, socket);
}

