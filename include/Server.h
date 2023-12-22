#pragma once
#include<map>
class Connection;
class EventLoop;
class Socket;
class Acceptor;
class Server
{
private:
    EventLoop *loop;
    Acceptor *acceptor;
    std::map<int,Connection*> connections;
public:
    Server(EventLoop*);
    ~Server();

    void handleReadEvent(int);
    void newConnetion(Socket *serv_sock);
    void deleteConnection(Socket *sock);
};
