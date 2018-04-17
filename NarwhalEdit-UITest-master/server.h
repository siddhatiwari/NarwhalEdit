
#ifndef SERVER_H
#define SERVER_H
#include <QTcpServer>
#include <QTcpSocket>
#include <vector>

class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0);
    ~Server();
    void startServer(int port);
    std::vector<QTcpSocket *> sockets;

signals:
    void dataReceived(QByteArray data, QTcpSocket *sender);

private slots:
    void newConnection();
    void disconnected();
    void readyRead();

};

#endif // SERVER_H
