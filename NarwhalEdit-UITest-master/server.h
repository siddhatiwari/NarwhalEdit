
#ifndef SERVER_H
#define SERVER_H
#include <QTcpServer>
#include <vector>
#include "mythread.h"

class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0);
    ~Server();
    void startServer();
    std::vector<QTcpSocket *> sockets;

signals:
    void dataReceived(QByteArray data);

private slots:
    void newConnection();
    void disconnected();
    void readyRead();

protected:
    //void incomingConnection(qintptr socketDescriptor);
};

#endif // SERVER_H
