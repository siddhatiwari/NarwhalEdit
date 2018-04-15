#include "mythread.h"


MyThread::MyThread(int ID, QObject *parent) : QThread(parent)
{
    socketDescriptor = ID;
}

void MyThread::run()
{
    qDebug() << socketDescriptor << "Starting thread";

    socket = new QTcpSocket();
    if (!socket->setSocketDescriptor(socketDescriptor)) {
        emit error(socket->error());
        return;
    }

    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::DirectConnection);
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));

    qDebug() << socketDescriptor << "Client thread";

    exec();
}

void MyThread::readyRead()
{
    QByteArray data = socket->readAll();

    qDebug() << socketDescriptor << "Data in: " << data;

    socket->write(data);
}

void MyThread::disconnected()
{
    qDebug() << socketDescriptor << "Disconnected";

    socket->close();
    exit(0);
}
