
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

    /**
     * @brief Starts the server on a port
     * @param int
     * @return none
     */

    void startServer(int port);
    std::vector<QTcpSocket *> sockets;
    int connections = 0;

signals:
    void dataReceived(QByteArray data, QTcpSocket *sender);

private slots:

    /**
     * @brief Handles a new connection
     * @return none
     */

    void newConnection();

    /**
     * @brief Handles a server disconnection
     * @return none
     */

    void disconnected();

    /**
     * @brief Handles if there is data ready to read
     * @return none
     */

    void readyRead();

};

#endif // SERVER_H
