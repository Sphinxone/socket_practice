#pragma once
#include<functional>

class Socket;
class EventLoop;
class Address;
class Channel;
class Acceptor
{
private:
    Socket *sock;
    EventLoop *loop;
    Channel *acceptChannel;
    Address *address;
    std::function<void(Socket *)> newConnectionCallback;
public:
    Acceptor(/* args */);
    Acceptor(EventLoop *_loop, Address *_address);
    ~Acceptor();
    // int accpet();
    void acceptConnection();
    void setNewConnectionCallback(std::function<void(Socket*)>);
};
