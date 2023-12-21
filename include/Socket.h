#pragma once
#include <assert.h>
#include <sys/socket.h>
#include <Address.h>

class Socket
{
private:
    int fd;

public:
    Socket();
    Socket(int af, int sock);
    Socket(int _fd);
    ~Socket();

    void bind(Address *address);
    void listen();
    void setnonblocking();
    int accept(Address *address);
    int getFd();
};
