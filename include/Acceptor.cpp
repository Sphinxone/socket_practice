#include "Acceptor.h"
#include "Address.h"
#include "Channel.h"
#include "EventLoop.h"
#include "Socket.h"

Acceptor::Acceptor()
{
}

Acceptor::Acceptor(EventLoop *_loop, Address *_address)
{
    loop = _loop;
    address = _address;
    sock = new Socket(AF_INET,SOCK_STREAM);
    address = _address;
    sock->bind(address);
    sock->listen();
    sock->setnonblocking();
    acceptChannel = new Channel(loop, sock->getFd());
    std::function<void()> cb = std::bind(&Acceptor::acceptConnection,this);
    acceptChannel->setCallback(cb);
    acceptChannel->enableReading();
    delete address;
}

Acceptor::~Acceptor()
{
    delete sock;
    delete address;
    delete acceptChannel;
}

void Acceptor::acceptConnection()
{
    newConnectionCallback(sock);
}

void Acceptor::setNewConnectionCallback(std::function<void(Socket *)> _cb)
{
    newConnectionCallback = _cb;
}
